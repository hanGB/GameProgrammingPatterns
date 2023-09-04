#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_game.h"
#include "per_object.h"
#include "object_pool.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"

PERWorld::PERWorld(ObjectPool* objectPool, GameMode* mode)
{
	PERLog::Logger().Info("월드 생성");

	m_objectPool = objectPool;
	m_gameMode = mode;

	m_objects.reserve(PER_DEFAULT_MAX_OBJECTS);
}

PERWorld::~PERWorld()
{
	PERLog::Logger().Info("월드 삭제");
}

void PERWorld::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);

	ProcessPendingMessage();
}

void PERWorld::UIUpdate(PERAudio& audio, double dTime)
{
	m_gameMode->GetHud().Update(audio, dTime);
}

void PERWorld::ObjectsInputUpdate(PERController& controller, PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetInput().Update(*m_objects[i], *this, controller, audio, dTime);
	}
}

void PERWorld::ObjectsAiUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetAi().Update(*m_objects[i], *this, audio, dTime);
	}
}

void PERWorld::ObjectsPhysicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetPhysics().Update(*m_objects[i], *this, audio, dTime);
	}
}

void PERWorld::ObjectsGraphicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetGraphics().Update(*m_objects[i], audio, dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer, double frameGap)
{
	for (int i = 0; i < m_sortedObjects.size(); ++i) {
		m_sortedObjects[i]->GetGraphics().Render(*m_sortedObjects[i], renderer, frameGap);
	}
}

void PERWorld::UIRender(PERRenderer& renderer)
{
	m_gameMode->GetHud().Renderer(renderer);
}

void PERWorld::Enter()
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 0.0, 0.0));
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
}

void PERWorld::Resume()
{
}

void PERWorld::RequestAddObject(PERObject* parent, PERObjectType type, PERVec3 position, PERVec3 currentAccel, PERStat stat, double lifeTime)
{
	PERWorldMessage message;
	message.id = PERWorldMessageId::ADD_OBJECT;
	message.object = parent;
	message.type = type;
	message.position = position;
	message.currentAccel = currentAccel;
	message.lifeTime = lifeTime;
	message.stat = stat;

	if (m_maxPending == m_numPending) ResizePedingArray();
	m_pending[m_numPending] = message;
	m_numPending++;
}

void PERWorld::RequestDeleteObject(PERObject* object)
{
	PERWorldMessage message;
	message.id = PERWorldMessageId::DELETE_OBJECT;
	message.object = object;

	if (m_maxPending == m_numPending) ResizePedingArray();
	m_pending[m_numPending] = message;
	m_numPending++;
}

bool PERWorld::CheckCollision(PERObject& object, double dTime)
{
	bool collided = false;

	// 본인 오브젝트 정보 얻기
	PERObjectType type = object.GetObjectType();
	PERVec3 position = object.GetPosition(), size = object.GetSize();
	PERBoundingType boundingtype = object.GetBoundingType();

	int id = object.GetIDInWorld();
	for (int i = 0; i < m_numObject; ++i) {
		// id가 같은 거(본인), 부모, 자식 건너뜀
		if (id == i) continue;
		if (object.GetParent() == m_objects[i]) continue;
		if (&object == (m_objects[i]->GetParent())) continue;

		// 같은 높이에 있지 않을 경우 충돌 불가하여 건너뜀
		if ((int)(position.z) != (int)(m_objects[i]->GetPosition().z)) continue;

		// 전에 충돌된 오브젝트가 같을 경우 충돌 오브젝트 제거
		if (object.GetCollidedObject() == m_objects[i]) object.SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
		if (&object == m_objects[i]->GetCollidedObject()) m_objects[i]->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

		// 충돌 처리 설정 -> type(충돌 검사를 요청한 오브젝트의 타입)은 fixed object, trigger가 될 수 없음
		// 
		// movable object && movable object == 완전 무시
		// movable object && trigger == 완전 무시
		// bullet && trigger == 완전 무시
		// 
		// movable object && fixed object == fixed 오브젝트에 합쳐짐(변경 될 수 있음)
		// movable object && (player || monster) == 물리적 처리
		// (player || monster) && fixed object  == 물리적 처리
		// 
		// 나머지 관계 == 물리적 처리 없는 충돌 처리

		// 상대 오브젝트 타입 얻기
		PERObjectType otherType = m_objects[i]->GetObjectType();
		PERVec3 otherPos = m_objects[i]->GetPosition(), otherSize = m_objects[i]->GetSize();
		PERBoundingType otherBoundingType = m_objects[i]->GetBoundingType();
		
		// 충돌 처리 무시 항목
		if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::MOVABLE_BLOCK) continue;
		else if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::TRIGGER) continue;
		else if (type == PERObjectType::BULLET && otherType == PERObjectType::TRIGGER) continue;

		if (otherBoundingType == PERBoundingType::RECTANGLE && boundingtype == PERBoundingType::RECTANGLE) {
			if (CheckAABBCollision(position, size, otherPos, otherSize)) {
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
				else ProcessCollisionWithoutMoving(object, type, *m_objects[i], otherType);
				
			}
		}
	}
	return collided;
}

void PERWorld::DoGarbegeCollection(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) {
		if (m_objects[i]->IsLifeTimeIsEnd(dTime)) {
			RequestDeleteObject(m_objects[i]);
		}
	}
}

void PERWorld::ProcessPendingMessage()
{
	for (int i = 0; i < m_numPending; ++i) {
		PERWorldMessage message = m_pending[i];

		switch (message.id) {
		case PERWorldMessageId::ADD_OBJECT: {
			PERObject* object = AddAndGetObject(message.type);
			object->SetPosition(message.position);
			object->SetCurrentAccel(message.currentAccel);
			object->SetLifeTime(message.lifeTime);
			object->SetParent(message.object);
			object->GetObjectState().SetStat(message.stat);
			break;
		}
		case PERWorldMessageId::DELETE_OBJECT: 
			DeleteObject(message.object);
			break;
		}
	}
	m_numPending = 0;
}

void PERWorld::AddObject(PERObject* object)
{
	if (m_objects.size() == m_numObject) {
		m_objects.push_back(object);
	}
	else {
		m_objects[m_numObject] = object;
	}
	object->SetIDInWorld(m_numObject);
	m_numObject++;

	m_isUpdateSortedObject = false;
}

void PERWorld::DeleteObject(PERObject* object)
{
	m_numObject--;
	int id = object->GetIDInWorld();
	m_objects[id] = m_objects[m_numObject];
	m_objects[id]->SetIDInWorld(id);
	m_objectPool->PushObject(object->GetObjectType(), object);

	m_isUpdateSortedObject = false;
}

PERObject* PERWorld::AddAndGetObject(PERObjectType type)
{
	PERObject* object = m_objectPool->PopObject(type);
	AddObject(object);
	return object;
}

void PERWorld::InitWorldObject()
{
	PERObject* monster;
	/*for (double x = -1.0; x < 1.0; x += 1.0) {
		for (double y = -1.0; y < 1.0; y += 1.0) {
			monster = m_objectPool->PopObject(PERObjectType::MONSTER);
			monster->SetPosition(PERVec3(x, y, 2.0));
			AddObject(monster);
		}
	}*/

	monster = m_objectPool->PopObject(PERObjectType::MONSTER);
	monster->SetPosition(PERVec3(-3.0, -3.0, 0.0));
	AddObject(monster);

	PERObject* block;
	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(3.0, 1.0, 0.0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-3.0, 1.0, 0.0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(1.0, 3.0, 0.0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(1.0, -3.0, 0.0));
	AddObject(block);

	PERObject* wall;
	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-1.0, 0.0, 1.1));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 1.2));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 1.3));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-5.0, 0.0, 0.1));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	AddObject(wall);

	PERLog::Logger().InfoWithFormat("월드 내 오브젝트 수: %d", m_numObject);
}

void PERWorld::UpdateSortedObjects()
{
	if (m_isUpdateSortedObject) return;

	m_sortedObjects.clear();

	// 위치의 z값이 작은 순서로 정렬
	m_sortedObjects.resize((size_t)m_numObject);
	std::copy(m_objects.begin(), m_objects.begin() + m_numObject, m_sortedObjects.begin());
	std::sort(m_sortedObjects.begin(), m_sortedObjects.end(), [](PERObject* a, PERObject* b) {
		return a->GetPosition().z < b->GetPosition().z;
		});

	m_isUpdateSortedObject = true;
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
	fixedObject.GetPhysics().ProcessCollision(fixedObject, movableObject, collisionVelocity, fixedVel, 0.0);
	movableObject.GetPhysics().ProcessCollision(movableObject, fixedObject, collisionVelocity, PERVec3(0.0, 0.0, movableVel.z), dTime * 1.5);
}

void PERWorld::ProcessCollisionWithoutMoving(PERObject& aObject, PERObjectType aType, PERObject& bObject, PERObjectType bType)
{
	// bullet && bullet == 물리적 처리만 무시, bullet 삭제
	// bullet && fixed object == 물리적 처리만 무시, bullet 삭제
	// bullet && movable object == 물리적 처리만 무시, bullet 삭제
	// bullet && (player || monster) == 물리적 처리만 무시, bullet 삭제, 데미지 처리
	// bullet && trigger == 완전 무시
	// (player || monster) && (player || monster) == 물리적 처리만 무시, 데미지 처리		 
	// (player || monster) && trigger == 물리적 처리만 무시

	// 총알간 상쇄
	if (aType == PERObjectType::BULLET && bType == PERObjectType::BULLET) {
		aObject.SetLifeTime(-1.0);
		bObject.SetLifeTime(-1.0);
	}
	// 총알 데미지 처리
	else if (aType == PERObjectType::BULLET) {
		if (bType == PERObjectType::TRIGGER) return;
		aObject.SetLifeTime(-1.0);
		bObject.GetObjectState().GiveDamage(bObject,
			aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);
	}
	else if (bType == PERObjectType::BULLET) {
		if (aType == PERObjectType::TRIGGER) return;
		bObject.SetLifeTime(-1.0);
		bObject.GetObjectState().GiveDamage(bObject,
			aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);
	}
	// 나머지
	else if (aType == PERObjectType::PLAYER && bType == PERObjectType::MONSTER) {
		aObject.GetObjectState().GiveDamage(aObject, bObject.GetObjectState().GetCollisionDamage(), 0);
	}
	else if (bType == PERObjectType::PLAYER && aType == PERObjectType::MONSTER) {
		bObject.GetObjectState().GiveDamage(bObject, aObject.GetObjectState().GetCollisionDamage(), 0);
	}
}