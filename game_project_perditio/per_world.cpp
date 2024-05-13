#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_game.h"
#include "per_object.h"
#include "object_storage.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "black_board.h"
#include "per_component.h"
#include "stuck_physics_component.h"
#include "spawner_ai_component.h"
#include "per_particle_pool.h"
#include "graphics_component.h"
#include "creating_particles_ai_component.h"

PERWorld::PERWorld()
{
	m_particlePool = new PERParticlePool();
}

PERWorld::~PERWorld()
{
	PERLog::Logger().Info("월드 삭제");

	// 가져온 오브젝트 전부 반환
	ReturnObejctToStorage();
	ClearObejctVector();

	// 객체 삭제
	delete m_gameMode;
	delete m_particlePool;

	// 이벤트 큐 삭제
	delete[] m_pending;
}

void PERWorld::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);
	SleepAndWakeupObjects();

	ProcessPendingMessage();

	m_gameMode->Update();
}

void PERWorld::UIUpdate(PERController& controller, PERAudio& audio, double dTime)
{
	m_gameMode->GetHud().Update(controller, audio, dTime);
}

void PERWorld::InputUpdate(PERController& controller, PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_inputComponents[i]->Update(*this, controller, audio, dTime);
	}
}

void PERWorld::AiUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_aiComponents[i]->Update(*this, audio, dTime);
	}
}

void PERWorld::PhysicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_physicsComponents[i]->Update(*this, audio, dTime);
	}
	m_particlePool->Update(dTime);
}

void PERWorld::GraphicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_graphicsComponents[i]->Update(GetHud(), audio, dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer, double frameGap)
{
	for (size_t i = 0; i < m_sortedGraphicsComponents.size(); ++i) {
		m_sortedGraphicsComponents[i]->Render(renderer, frameGap);
	}
	m_particlePool->Renderer(renderer);
	GetHud().RendererInWorld(renderer, *m_database);
}

void PERWorld::UIRender(PERRenderer& renderer)
{
	GetHud().Renderer(renderer, *m_database);
}

void PERWorld::Enter()
{
	AddObject(&m_gameMode->GetPlayer());

	InitWorldObject();

	m_gameMode->StartUse();
}

void PERWorld::Exit()
{
	m_gameMode->EndUse();
}

void PERWorld::Pause()
{
	m_gameMode->EndUse();
}

void PERWorld::Resume()
{
	m_gameMode->StartUse();
}

void PERWorld::RequestAddObject(PERObject* parent, PERObjectType type, const char* databaseId, PERDatabaseType databaseType, 
	PERStat stat, PERVec3 position, double lifeTime, PERVec3 currentAccel)
{
	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].id = PERWorldMessageId::ADD_OBJECT;
	m_pending[m_numPending].object = parent;
	m_pending[m_numPending].type = type;
	m_pending[m_numPending].databaseId = databaseId;
	m_pending[m_numPending].databaseType = databaseType;
	m_pending[m_numPending].position = position;
	m_pending[m_numPending].currentAccel = currentAccel;
	m_pending[m_numPending].lifeTime = lifeTime;
	m_pending[m_numPending].stat = stat;

	m_numPending++;
}

void PERWorld::RequestDeleteObject(PERObject* object)
{
	RequestSimpleDoObject(object, PERWorldMessageId::DELETE_OBJECT);
}

bool PERWorld::CheckCollision(PERObject& object, double dTime)
{
	bool collided = false;
	bool isOnPlatform = false;

	// 본인 오브젝트 정보 얻기
	PERObjectType type = object.GetObjectType();
	PERVec3 position = object.GetBoundingBoxPosition(), size = object.GetBoundingBoxSize();
	PERBoundingType boundingtype = object.GetBoundingType();

	int id = object.GetIDInWorld();
	for (int i = 0; i < m_numObject; ++i) {
		// 본인이 죽었거나 죽은 오브젝트 건너뜀
		if (object.GetLifeTime() < 0.0) return collided;
		if (m_objects[i]->GetLifeTime() < 0.0) continue;

		// id가 같은 거(본인), 부모, 자식 건너뜀
		if (id == i) continue;
		if (object.GetParent() == m_objects[i]) continue;
		if (&object == (m_objects[i]->GetParent())) continue;

		// 상대의 z값이 나보다 높으면 건너뜀
		if ((int)(floor(m_objects[i]->GetPosition().z)) > (int)floor(position.z)) continue;
		// 상대의 z값 + 1이 나보다 작으면 건너뜀
		if ((int)(floor(m_objects[i]->GetPosition().z)) + 1 < (int)floor(position.z)) continue;

		// 전에 충돌된 오브젝트가 같을 경우 충돌 오브젝트 제거
		if (object.GetCollidedObject() == m_objects[i]) object.SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
		if (&object == m_objects[i]->GetCollidedObject()) m_objects[i]->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

		// 충돌 처리 설정 -> type(충돌 검사를 요청한 오브젝트의 타입)은 fixed object, trigger가 될 수 없음
		// 
		// movable object && movable object == 완전 무시
		// movable object && trigger == 완전 무시
		// bullet(blade) && trigger == 완전 무시
		// 
		// movable object && fixed object == fixed 오브젝트에 합쳐짐(변경 될 수 있음)
		// movable object && (player || monster) == 물리적 처리
		// (player || monster) && fixed object  == 물리적 처리
		// 
		// 나머지 관계 == 물리적 처리 없는 충돌 처리

		// 상대 오브젝트 타입 얻기
		PERObjectType otherType = m_objects[i]->GetObjectType();
		PERVec3 otherPos = m_objects[i]->GetBoundingBoxPosition(), otherSize = m_objects[i]->GetBoundingBoxSize();
		PERBoundingType otherBoundingType = m_objects[i]->GetBoundingType();

		// 특수 타입 처리
		// 문은 고정 블럭으로 처리
		if (otherType == PERObjectType::DOOR) otherType = PERObjectType::FIXED_BLOCK;
		else if (otherType == PERObjectType::BUTTON) continue;
		else if (otherType == PERObjectType::PARTICLE_EFFECTER) continue;
		else if (otherType == PERObjectType::SPAWNER) continue;
		// 트리거는 본인만 검사하도록 other object인 상태에서는 넘김
		else if (otherType == PERObjectType::TRIGGER) continue;

		// 충돌 처리 무시 항목
		if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::MOVABLE_BLOCK) continue;
		else if ((type == PERObjectType::BULLET || type == PERObjectType::BLADE) && otherType == PERObjectType::PRESSURE) continue;
		else if ((otherType == PERObjectType::BULLET || otherType == PERObjectType::BLADE || otherType == PERObjectType::FIXED_BLOCK) 
			&& type == PERObjectType::TRIGGER) continue;

		if (otherBoundingType == PERBoundingType::RECTANGLE && boundingtype == PERBoundingType::RECTANGLE) {
			if (CheckAABBCollision(position, size, otherPos, otherSize)) {
				// 상대가 나보다 바로 아래 있고 고정 블럭일 경우 플랫폼 위에 있다고 설정 후 건너뜀(땅)
				if ((int)(floor(otherPos.z)) == (int)floor(position.z) -1 && otherType == PERObjectType::FIXED_BLOCK) {
					isOnPlatform = true;  
					continue;
				}
				// 충돌로 설정
				collided = true;

				// 물리적 처리 항목
				if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::FIXED_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if (type == PERObjectType::MOVABLE_BLOCK && (otherType == PERObjectType::PLAYER || otherType == PERObjectType::MONSTER))
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if ((type == PERObjectType::PLAYER || type == PERObjectType::MONSTER) && otherType == PERObjectType::MOVABLE_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if ((type == PERObjectType::PLAYER || type == PERObjectType::MONSTER) && otherType == PERObjectType::FIXED_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);

				// 물리적 처리 없이 다른 상호 작용
				else ProcessCollisionWithoutMoving(object, type, *m_objects[i], otherType, dTime);

			}
		}
	}

	// 플랫폼 위에 있지 않을 경우 떨어져 죽음
	if (!isOnPlatform) {
		// 임시로 라이프 타임이 0보다 작도록 설정해 단순히 오브젝트 제거
		object.GetObjectState().KillSelf(*this);
	}

	return collided;
}

PERHud& PERWorld::GetHud()
{
	return m_gameMode->GetHud();
}

PERDatabase& PERWorld::GetDatabase()
{
	return *m_database;
}

PERParticlePool& PERWorld::GetParticlePool()
{
	return *m_particlePool;
}

GameMode& PERWorld::GetGameMode()
{
	return *m_gameMode;
}

void PERWorld::DoGarbegeCollection(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) {
		if (m_objects[i]->IsLifeTimeEnd(dTime)) 
		{
			InformObjectDeadToSpawner(m_objects[i]);
			RequestDeleteObject(m_objects[i]);
		}
	}
}

void PERWorld::InformObjectDeadToSpawner(PERObject* object)
{
	PERObject* parent = object->GetParent();
	// 부모가 있을 경우
	if (parent)
	{	// 부모가 스포너인 경우 스포너에게 알림
		if ( parent->GetObjectType() == PERObjectType::SPAWNER)
		{
			dynamic_cast<SpawnerAiComponent*>(&parent->GetAi())->InformSpawnedObjectIsDead();
		}
	}
}

void PERWorld::SleepAndWakeupObjects()
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	// sleep
	for (int i = 0; i < m_numObject; ++i) {
		if (m_objects[i]->IsHaveToSleep(playerPos)) {
			RequestSimpleDoObject(m_objects[i], PERWorldMessageId::SLEEP_OBJECT);
		}
	}
	// wake up
	for (int i = 0; i < m_numSleepObject; ++i) {
		if (!m_sleepObjects[i]->IsHaveToSleep(playerPos)) {
			RequestSimpleDoObject(m_sleepObjects[i], PERWorldMessageId::WAKE_UP_OBJECT);
		}
	}
}

void PERWorld::ProcessPendingMessage()
{
	for ( int i = 0; i < m_numPending; ++i ) {
		PERWorldMessage message = m_pending[ i ];

		switch ( message.id ) {
		case PERWorldMessageId::ADD_OBJECT: {
			ProcessAddMessage(message);
			break;
		}
		case PERWorldMessageId::DELETE_OBJECT: {
			DeleteObject(message.object);
			break;
		}
		case PERWorldMessageId::SLEEP_OBJECT: {
			SleepObject(message.object);
			break;
		}
		case PERWorldMessageId::WAKE_UP_OBJECT: {
			WakeUpObject(message.object);
			break;
		}
		}
	}
	m_numPending = 0;
}

void PERWorld::RequestSimpleDoObject(PERObject* object, const PERWorldMessageId& message)
{
	if ( m_maxPending == m_numPending ) ResizePedingArray();

	m_pending[ m_numPending ].id = message;
	m_pending[ m_numPending ].object = object;
	m_numPending++;
}

void PERWorld::ProcessAddMessage(PERWorldMessage& message)
{
	PERObject* newObject = AddAndGetObject(message.type);

	// 데이터베이스 id가 이펙트 id인 경우
	if ( message.databaseType == PERDatabaseType::EFFECT )
	{
		EffectData* eData = m_database->GetEffectData(message.databaseId.c_str());
		SetForAddParticleEffecterMessage(message, newObject, eData);
		return;
	}
	// 데이터베이스 id가 몬스터 id인 경우
	else if ( message.databaseType == PERDatabaseType::MONSTER )
	{
		newObject->GetObjectState().SetNameId(m_database->GetMonsterData(message.databaseId.c_str())->nameId);
		message.databaseId = m_database->GetMonsterData(message.databaseId.c_str())->visualId;
	}
	// 데이터베이스 id가 비주얼 id인 경우로 취급(아닐 경우 무조건 오류)
	VisualData* vData = m_database->GetVisualData(message.databaseId.c_str());
	// 기본
	SetBaseOfAddMessage(message, newObject, vData);
	// 칼날일 경우
	if ( message.type == PERObjectType::BLADE ) SetForAddBladeMessage(message, newObject);
	// 소환기로 인해 소환된 오브젝트인 경우
	if (newObject->GetParent())
	{
		if (newObject->GetParent()->GetObjectType() == PERObjectType::SPAWNER )
		{
			SetForAddByObjectMessage(message, newObject);
		}
	}
}

void PERWorld::SetBaseOfAddMessage(PERWorldMessage& message, PERObject* newObject, VisualData* vData)
{
	// 그래픽 설정
	PERComponent::GraphicsData gData;
	gData.shape = vData->shape; gData.color = vData->color;
	gData.border = vData->borderOn; gData.borderWidth = vData->borderWidth; gData.borderColor = vData->borderColor;

	newObject->SetSize(vData->size);
	newObject->SetMass(vData->mass);
	newObject->GetGraphics().SetData(gData);

	// 메세지로 받은 데이터 설정
	newObject->SetPosition(message.position);
	newObject->SetCurrentAccel(message.currentAccel);
	newObject->SetLifeTime(message.lifeTime);
	newObject->SetParent(message.object);
	newObject->GetObjectState().SetStat(message.stat);
}

void PERWorld::SetForAddParticleEffecterMessage(PERWorldMessage& message, PERObject* newObject, EffectData* eData)
{
	// 스탯의 레벨을 이펙트 타입으로 사용
	AiData data;
	data.particleEffectType = (PERParticleEffectType) message.stat.level;
	newObject->GetAi().SetData(data);

	// 스탯의 바디값을 플레이어의 흡수 여부 설정
	bool isColletedByPlayer = (bool)message.stat.body;

	dynamic_cast< CreatingParticlesAiComponent* >( &newObject->GetAi() )->SetParticle(
		eData->shape, eData->size, eData->amount,
		eData->speedRate, eData->spawnDelay, eData->particleLifeTime,
		isColletedByPlayer,
		eData->color,
		eData->borderOn, eData->borderWidth, eData->borderColor
	);

	// 메세지로 받은 데이터 설정
	newObject->SetPosition(message.position);
	newObject->SetLifeTime(message.lifeTime);
	newObject->SetParent(nullptr);
}

void PERWorld::SetForAddBladeMessage(PERWorldMessage& message, PERObject* newObject)
{
	// 위치값을 붙여진 위치로 설정
	PERComponent::PhysicsData pData;
	pData.stuckPosition = message.position;
	newObject->GetPhysics().SetData(pData);

	// 바로 슬립되는 것을 방지하기 위해 위치를 플레이어 위치로 임시 설정
	PERVec3 pos = BlackBoard::GetPlayerPos();
	newObject->SetPosition(pos);
}

void PERWorld::SetForAddByObjectMessage(PERWorldMessage& message, PERObject* newObject)
{
	// 소환된 오브젝트의 스폰 위치를 현재 위치로 함
	newObject->SetCurrentPositionToSpawnPosition();
}

void PERWorld::SleepObject(PERObject* object)
{
	// 오브젝트 제거
	m_numObject--;
	int id = object->GetIDInWorld();
	// 맨 뒤 컨포넌트 이동
	m_inputComponents[id] = m_inputComponents[m_numObject];
	m_aiComponents[id] = m_aiComponents[m_numObject];
	m_physicsComponents[id] = m_physicsComponents[m_numObject];
	m_graphicsComponents[id] = m_graphicsComponents[m_numObject];
	// 맨 뒤 오브젝트 이동 및 아이디 변경
	m_objects[id] = m_objects[m_numObject];
	m_objects[id]->SetIDInWorld(id);

	m_isUpdateSortedGraphicsComponent = false;

	// 슬립 오브젝트 벡터에 추가
	if (m_sleepObjects.size() == m_numSleepObject) {
		m_maxSleepObject *= 2;
		m_sleepObjects.reserve(m_maxSleepObject);
		m_sleepObjects.resize(m_maxSleepObject);
	}
	m_sleepObjects[m_numSleepObject] = object;
	object->SetIDInWorld(m_numSleepObject);
	m_numSleepObject++;
}

void PERWorld::WakeUpObject(PERObject* object)
{
	// 슬립 오브젝트 벡터에서 제거
	m_numSleepObject--;
	int id = object->GetIDInWorld();
	// 맨 뒤 오브젝트 이동 및 아이디 변경
	m_sleepObjects[id] = m_sleepObjects[m_numSleepObject];
	m_sleepObjects[id]->SetIDInWorld(id);

	// 오브젝트 추가
	AddObject(object);
}

void PERWorld::AddObject(PERObject* object)
{
	if (m_objects.size() == m_numObject) {
		m_maxObject *= 2;
		m_objects.reserve(m_maxObject);
		m_objects.resize(m_maxObject);
		// 컨포넌트들도 리사이즈
		m_inputComponents.resize(m_maxObject);
		m_aiComponents.resize(m_maxObject);
		m_physicsComponents.resize(m_maxObject);
		m_graphicsComponents.resize(m_maxObject);
	}
	m_inputComponents[m_numObject]		= &object->GetInput();
	m_aiComponents[m_numObject]			= &object->GetAi();
	m_physicsComponents[m_numObject]	= &object->GetPhysics();
	m_graphicsComponents[m_numObject]	= &object->GetGraphics();

	m_objects[m_numObject] = object;
	object->SetIDInWorld(m_numObject);
	m_numObject++;

	m_isUpdateSortedGraphicsComponent = false;
}

void PERWorld::DeleteObject(PERObject* object)
{
	m_numObject--;
	int id = object->GetIDInWorld();
	// 맨 뒤 컨포넌트 이동
	m_inputComponents[id]		= m_inputComponents[m_numObject];
	m_aiComponents[id]			= m_aiComponents[m_numObject];
	m_physicsComponents[id]		= m_physicsComponents[m_numObject];
	m_graphicsComponents[id]	= m_graphicsComponents[m_numObject];
	// 맨 뒤 오브젝트 이동 및 아이디 변경
	m_objects[id] = m_objects[m_numObject];
	m_objects[id]->SetIDInWorld(id);
	m_objectStorage->PushObject(object->GetObjectType(), object);

	m_isUpdateSortedGraphicsComponent = false;
}

PERObject* PERWorld::AddAndGetObject(PERObjectType type)
{
	PERObject* object = m_objectStorage->PopObject(type);
	AddObject(object);
	return object;
}

void PERWorld::InitWorldObject()
{
	// 네비게이션 데이터에 영향을 주는 오브젝트 먼저 추가
	AddFixedAndPhysicalObjects();

	// 네비게이션 데이터 설정
	BlackBoard::GetNavigationData().InitCells();
	BlackBoard::GetNavigationData().SetCells(m_objects, m_numObject);

	// 네비게이션 데이터와 상관없는 나머지 오브젝트 추가
	AddOtherObjects();

	PERLog::Logger().InfoWithFormat("월드 내 오브젝트 수: %d", m_numObject);
}

void PERWorld::InitSettingForWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode)
{
	PERLog::Logger().Info("월드 생성");

	m_objectStorage = objectStorage;
	m_database = database;
	m_gameMode = mode;

	// 오브젝트, 컨포넌트 벡터 리사이즈
	m_objects.reserve(m_maxObject);
	m_objects.resize(m_maxObject);
	m_inputComponents.resize(m_maxObject);
	m_aiComponents.resize(m_maxObject);
	m_physicsComponents.resize(m_maxObject);
	m_graphicsComponents.resize(m_maxObject);
}

void PERWorld::SetObjectVisual(PERObject* object, const char* visualId)
{
	VisualData* data = m_database->GetVisualData(visualId);

	object->SetSize(data->size);
	object->SetMass(data->mass);
	object->SetBoundingType(data->boundingType);
	SetObjectShapeAndColor(object, data->shape, data->color, data->borderOn, data->borderWidth, data->borderColor);
}

void PERWorld::SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
	bool border, int borderWidth, PERColor borderColor)
{
	PERComponent::GraphicsData graphicsData;
	graphicsData.shape = shape; graphicsData.color = color;
	graphicsData.border = border; graphicsData.borderWidth = borderWidth; graphicsData.borderColor = borderColor;

	object->GetGraphics().SetData(graphicsData);
}

void PERWorld::UpdateSortedGraphicsComponents()
{
	if (m_isUpdateSortedGraphicsComponent) return;

	m_sortedGraphicsComponents.clear();

	// 위치의 z값이 작은 순서로 정렬
	m_sortedGraphicsComponents.resize((size_t)m_numObject);
	std::copy(m_graphicsComponents.begin(), m_graphicsComponents.begin() + m_numObject, m_sortedGraphicsComponents.begin());
	std::sort(m_sortedGraphicsComponents.begin(), m_sortedGraphicsComponents.end(), [](GraphicsComponent* a, GraphicsComponent* b) {
		return a->GetPositionZValue() < b->GetPositionZValue();
		});

	m_isUpdateSortedGraphicsComponent = true;
}

void PERWorld::UpdateCamera(PERRenderer& renderer, double frameGap)
{
	// 게임 모드에 따라 카메라 업데이트
	m_gameMode->UpdateCamera(renderer, frameGap);
}

void PERWorld::ResizePedingArray()
{
	// 현재 배열 크기를 2배로 증가
	m_maxPending *= 2;
	PERWorldMessage* newArray = new PERWorldMessage[m_maxPending];
	memmove(newArray, m_pending, sizeof(m_pending));

	delete[] m_pending;
	m_pending = newArray;
}

bool PERWorld::CheckAABBCollision(PERVec3 aPos, PERVec3 aSize, PERVec3 bPos, PERVec3 bSize)
{
	PERVec3 aHalfSize = aSize * 0.5;
	PERVec3 bHalfSize = bSize * 0.5;

	if (aPos.x - aHalfSize.x > bPos.x + bHalfSize.x) return false;
	if (aPos.x + aHalfSize.x < bPos.x - bHalfSize.x) return false;
	if (aPos.y - aHalfSize.y > bPos.y + bHalfSize.y) return false;
	if (aPos.y + aHalfSize.y < bPos.y - bHalfSize.y) return false;
	
	return true;
}

void PERWorld::AdjustPositionWithObjects(PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime)
{
	// 이전 충돌로 인해 실제로 변경된 값 얻기
	PERVec3	aVel = aObject.GetCollidedVelocity(); double aMass = aObject.GetCollidedMass();
	PERVec3 bVel = bObject.GetCollidedVelocity(); double bMass = bObject.GetCollidedMass();

	// 충돌 처리(무거운 쪽을 고정된 걸로 생각)
	if (aMass > bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			aObject, aPos, aSize, aVel,
			bObject, bPos, bSize, bVel, dTime);
	}
	else if (aMass <= bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			bObject, bPos, bSize, bVel, 
			aObject, aPos, aSize, aVel, dTime);
	}
}

void PERWorld::ProcessCollisionBetweenFixedAndMovable(
	PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
	PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime)
{
	// 충돌 속도 계산
	PERVec3 collisionVelocity = movableVel - fixedVel;

	// 충돌 거리 계산
	PERVec3 fixedHalfSize = fixedSize * 0.5;
	PERVec3 movableHalfSize = movableSize * 0.5;

	PERVec3 collisionDistance = PERVec3(0.0, 0.0, 0.0);
	PERVec3 collisionTimeRate = PERVec3(0.0, 0.0, 0.0);

	if (collisionVelocity.x > 0.0) collisionDistance.x = (movablePos.x + movableHalfSize.x) - (fixedPos.x - fixedHalfSize.x);
	else if (collisionVelocity.x < 0.0) collisionDistance.x = (fixedPos.x + fixedHalfSize.x) - (movablePos.x - movableHalfSize.x);

	if (collisionVelocity.y > 0.0) collisionDistance.y = (movablePos.y + movableHalfSize.y) - (fixedPos.y - fixedHalfSize.y);
	else if (collisionVelocity.y < 0.0) collisionDistance.y = (fixedPos.y + fixedHalfSize.y) - (movablePos.y - movableHalfSize.y);

	if (collisionVelocity.x == 0.0) {
		if (collisionDistance.x == 0.0) collisionTimeRate.x = 0.0;
		else collisionTimeRate.x = 1.0;
	}
	else collisionTimeRate.x = std::abs(collisionDistance.x / collisionVelocity.x);

	if (collisionVelocity.y == 0.0) {
		if (collisionDistance.y == 0.0) collisionTimeRate.y = 0.0;
		else collisionTimeRate.y = 1.0;
	}
	else collisionTimeRate.y = std::abs(collisionDistance.y / collisionVelocity.y);

	double collisionTime = collisionTimeRate.x * dTime;
	if (collisionTimeRate.x < collisionTimeRate.y) collisionTime = collisionTimeRate.y * dTime;

	// 각 오브젝트에서 충돌 처리
	fixedObject.GetPhysics().ProcessCollision(movableObject, collisionVelocity, fixedVel, 0.0);
	movableObject.GetPhysics().ProcessCollision(fixedObject, collisionVelocity, PERVec3(0.0, 0.0, movableVel.z), dTime * 1.5);
}

void PERWorld::ProcessCollisionWithoutMoving(PERObject& aObject, PERObjectType aType, PERObject& bObject, PERObjectType bType, double dTime)
{
	// bullet(blade) && bullet(blade) == 물리적 처리만 무시, bullet(blade) 삭제
	// bullet(blade) && fixed object == 물리적 처리만 무시, bullet(blade) 삭제
	// bullet(blade) && movable object == 물리적 처리만 무시, bullet(blade) 삭제
	// bullet(blade) && (player || monster) == 물리적 처리만 무시, bullet(blade) 삭제, 데미지 처리
	// bullet(blade) && trigger == 완전 무시
	// (player || monster) && (player || monster) == 물리적 처리만 무시, 데미지 처리		 
	// (player || monster) && trigger == 물리적 처리만 무시

	// 총알이랑 칼날은 처리 방식이 동일하기 때문에 칼날을 총알로 취급함
	if (aType == PERObjectType::BLADE) aType = PERObjectType::BULLET;
	if (bType == PERObjectType::BLADE) bType = PERObjectType::BULLET;

	// 총알간 상쇄
	if (aType == PERObjectType::BULLET && bType == PERObjectType::BULLET) {
		aObject.SetLifeTime(-1.0);
		bObject.SetLifeTime(-1.0);
	}
	// 총알 데미지 처리
	else if (aType == PERObjectType::BULLET) {
		bObject.GetObjectState().GiveDamage(aObject, *this, aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);

		// 총알 속도 방향으로 약간 이동(넉백)
		if (aObject.GetObjectType() == PERObjectType::BULLET) {
			aObject.SetLifeTime(-1.0);
			bObject.GetPhysics().GiveForce(*this, NormalizeVector(aObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// 칼날의 상대적 위치 방향으로 약간 이동(넉백)
		else if (aObject.GetObjectType() == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(aObject.GetPhysics());
			bObject.GetPhysics().GiveForce(*this, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	else if (bType == PERObjectType::BULLET) {
		aObject.GetObjectState().GiveDamage(bObject, *this, bObject.GetObjectState().GetStat().physicalAttack, bObject.GetObjectState().GetStat().mindAttack);

		// 총알 속도 방향으로 약간 이동(넉백)
		if (bObject.GetObjectType() == PERObjectType::BULLET) {
			bObject.SetLifeTime(-1.0);
			aObject.GetPhysics().GiveForce(*this, NormalizeVector(bObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// 칼날의 상대적 위치 방향으로 약간 이동(넉백)
		else if (aObject.GetObjectType() == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(bObject.GetPhysics());
			aObject.GetPhysics().GiveForce(*this, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	// 나머지
	// 플레이어와 몬스터 간
	else if (aType == PERObjectType::PLAYER && bType == PERObjectType::MONSTER) {
		// 플레이어에게 대미지를 줌
		aObject.GetObjectState().GiveDamage(bObject, *this, bObject.GetObjectState().GetCollisionDamage(), 0);
	}
	else if (bType == PERObjectType::PLAYER && aType == PERObjectType::MONSTER) {
		bObject.GetObjectState().GiveDamage(aObject, *this, aObject.GetObjectState().GetCollisionDamage(), 0);
	}
	// 플레이어 또는 몬스터 또는 움직이는 오브젝트 와 압력 발판 간
	else if ((aType == PERObjectType::PLAYER || aType == PERObjectType::MONSTER || aType == PERObjectType::MOVABLE_BLOCK) && bType == PERObjectType::PRESSURE) {
		// 충돌 처리(필요한 건 본인 오브젝트 뿐, 내부적으로 발판이 눌려 입력된 것으로 처리됨)
		bObject.GetPhysics().ProcessCollision(aObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	else if ((bType == PERObjectType::PLAYER || bType == PERObjectType::MONSTER || bType == PERObjectType::MOVABLE_BLOCK) && aType == PERObjectType::PRESSURE) {
		aObject.GetPhysics().ProcessCollision(bObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	// 트리거와 플레이어간
	else if (aType == PERObjectType::PLAYER && bType == PERObjectType::TRIGGER) {
		// 충돌 처리
		bObject.GetPhysics().ProcessCollision(aObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	else if ( aType == PERObjectType::TRIGGER && bType == PERObjectType::PLAYER ) {
		// 충돌 처리
		aObject.GetPhysics().ProcessCollision(bObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
}

void PERWorld::ReturnObejctToStorage()
{
	for (int i = 0; i < m_numObject; ++i) {
		m_objectStorage->PushObject(m_objects[i]->GetObjectType(), m_objects[i]);
	}
	for (int i = 0; i < m_numSleepObject; ++i) {
		m_objectStorage->PushObject(m_sleepObjects[i]->GetObjectType(), m_sleepObjects[i]);
	}
}

void PERWorld::ClearObejctVector()
{
	// 모든 오브젝트 제거 
	m_objects.clear();
	m_sleepObjects.clear();

	m_inputComponents.clear();
	m_aiComponents.clear();
	m_physicsComponents.clear();
	m_graphicsComponents.clear();
	m_sortedGraphicsComponents.clear();

	// 메모리 재할당
	m_objects.shrink_to_fit();
	m_sleepObjects.shrink_to_fit();

	m_inputComponents.shrink_to_fit();
	m_aiComponents.shrink_to_fit();
	m_physicsComponents.shrink_to_fit();
	m_graphicsComponents.shrink_to_fit();
	m_sortedGraphicsComponents.shrink_to_fit();
}
