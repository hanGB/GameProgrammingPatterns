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

void PERWorld::Enter(PERRenderer& renderer, PERAudio& audio)
{
	AddObject(&m_gameMode->GetPlayer());
	InitWorldObject();
	m_gameMode->UpdateCamera(renderer, 0.0);
	m_gameMode->StartUse();

	// 배경음 재생
	audio.RequestHandleSound(PERAudioMessageId::PLAY_BGM);
	audio.RequestHandleSound(PERAudioMessageId::PLAY_ALL_AMBIENT_SOUNDS);
}

void PERWorld::Exit(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->EndUse();

	// 배경음 종료
	audio.RequestHandleSound(PERAudioMessageId::STOP_BGM);
	audio.RequestHandleSound(PERAudioMessageId::STOP_ALL_AMBIENT_SOUNDS);
}

void PERWorld::Pause(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->EndUse();

	// 배경음 멈춤
	audio.RequestHandleSound(PERAudioMessageId::PAUSE_BGM);
	audio.RequestHandleSound(PERAudioMessageId::PAUSE_ALL_AMBIENT_SOUNDS);
}

void PERWorld::Resume(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->UpdateCamera(renderer, 0.0);
	m_gameMode->StartUse();

	// 배경음 다시 재생
	audio.RequestHandleSound(PERAudioMessageId::RESUME_BGM);
	audio.RequestHandleSound(PERAudioMessageId::RESUME_ALL_AMBIENT_SOUNDS);

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

bool PERWorld::CheckCollision(PERObject& object, PERAudio& audio, double dTime)
{
	return PhysicsHelper::CheckCollisionBetweenOtherObjects(*this, audio, object, m_objects, m_numObject, nullptr, dTime);
}

bool PERWorld::CheckCollisionWithoutSpecificObject(PERObject& object, PERObject& exceptObject, PERAudio& audio, double dTime)
{
	return false;
	PhysicsHelper::CheckCollisionBetweenOtherObjects(*this, audio, object, m_objects, m_numObject, &exceptObject, dTime);
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

	// sleep(플레이어 제외)
	for (int i = 1; i < m_numObject; ++i) {
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
		PERWorldMessage& message = m_pending[i];

		switch (message.id) {
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
	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].id = message;
	m_pending[m_numPending].object = object;
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
	if ( message.databaseType == PERDatabaseType::MONSTER )
	{
		newObject->GetObjectState().SetNameId(m_database->GetMonsterData(message.databaseId.c_str())->nameId);
		message.databaseId = m_database->GetMonsterData(message.databaseId.c_str())->visualId;
	}
	// 데이터베이스 id가 비주얼 id인 경우로 취급(아닐 경우 무조건 오류)
	VisualData* vData = m_database->GetVisualData(message.databaseId.c_str());
	// 기본
	SetBaseOfAddMessage(message, newObject, vData);
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

int PERWorld::GetNumObject() const
{
	return m_numObject;
}

std::vector<PERObject*>& PERWorld::GetObjects()
{
	return m_objects;
}

void PERWorld::MakeFixedObjectsDataToFile(const char* fileName)
{
	PERLog::Logger().Info("맵 데이터 생성 시작");

	std::ofstream out(fileName, std::ios::binary);

	int num = GetNumObject();
	// 맨 처음 오브젝트인 플레이어 제외
	out << num - 1 << ' ';

	std::vector<PERObject*>& objects = GetObjects();
	for (int i = 1; i < num; ++i) {
		PERObject* object = objects[i];

		out << (int)object->GetObjectType() << ' ';
		out << object->GetPosition().x << ' ';
		out << object->GetPosition().y << ' ';
		out << object->GetPosition().z << ' ';
		out << object->GetSize().x << ' ';
		out << object->GetSize().y << ' ';
		out << object->GetSize().z << ' ';
		out << object->GetMass() << ' ';
		out << object->GetVisualId() << ' ';
	}

	out.close();

	PERLog::Logger().Info("맵 데이터 생성 완료");
}

void PERWorld::ReadFixedObjectsDataFromFile(const char* fileName)
{
	PERLog::Logger().Info("맵 데이터 읽기 시작");

	std::ifstream in(fileName, std::ios::binary);

	int num = 0;
	in >> num;

	for (int i = 0; i < num; ++i) {
		int objectType;
		PERVec3 pos, size;
		double mass;
		std::string visualId;
		in >> objectType;
		in >> pos.x; in >> pos.y; in >> pos.z;
		in >> size.x; in >> size.y; in >> size.z;
		in >> mass;
		in >> visualId;

		PERObject* object;
		object = m_objectStorage->PopObject((PERObjectType)objectType);
		SetObjectVisual(object, visualId.c_str());
		object->SetPosition(pos);
		object->SetSize(size);
		object->SetMass(mass);
		AddObject(object);
	}

	in.close();

	PERLog::Logger().Info("맵 데이터 읽기 완료");
}

PERObject* PERWorld::AddAndGetObject(PERObjectType type)
{
	PERObject* object = m_objectStorage->PopObject(type);
	AddObject(object);
	return object;
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
	object->SetVisualId(visualId);
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
