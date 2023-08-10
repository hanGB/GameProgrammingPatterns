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
}

void PERWorld::Exit()
{
}

void PERWorld::Pause()
{
}

void PERWorld::Resume()
{
}

void PERWorld::RequestAddObject(PERObject* parent, PERObjectType type, PERVec3 position, PERVec3 currentAccel, double lifeTime)
{
	PERWorldMessage message;
	message.id = PERWorldMessageId::ADD_OBJECT;
	message.object = parent;
	message.type = type;
	message.position = position;
	message.currentAccel = currentAccel;
	message.lifeTime = lifeTime;

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

	PERObjectType type = object.GetObjectType();
	PERVec3 position = object.GetPosition(), size = object.GetSize();

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

		// 충돌로 인해 실제로 변경된 값 얻기
		PERVec3	velocity = object.GetCollidedVelocity(); double mass = object.GetCollidedMass();
		PERBoundingType boundingtype = object.GetBoundingType();
		
		PERObjectType otherType = m_objects[i]->GetObjectType();
		PERVec3 otherPos = m_objects[i]->GetPosition(), otherSize = m_objects[i]->GetSize();
		// 충돌로 인해 실제로 변경된 값 얻기
		PERVec3 otherVel = m_objects[i]->GetCollidedVelocity(); double otherMass = m_objects[i]->GetCollidedMass();
		PERBoundingType otherBoundingType = m_objects[i]->GetBoundingType();
		
		if (otherBoundingType == PERBoundingType::RECTANGLE && boundingtype == PERBoundingType::RECTANGLE) {
			if (CheckAABBCollision(position, size, otherPos, otherSize)) {
				collided = true;
				
				//충돌 처리(무거운 쪽을 고정된 걸로 생각)
				if (mass > otherMass) {
					ProcessCollisionBetweenFixedAndMovable(
						object, position, size, velocity,
						*m_objects[i], otherPos, otherSize, otherVel, dTime);
				}
				else if (mass <= otherMass) {
					ProcessCollisionBetweenFixedAndMovable(
						*m_objects[i], otherPos, otherSize, otherVel, 
						object, position, size, velocity, dTime);
				}
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
	/*PERObject* monster;
	for (double x = -5.0; x < 5.0; x += 1.0) {
		for (double y = -5.0; y < 5.0; y += 1.0) {
			monster = m_objectPool->PopObject(PERObjectType::MONSTER);
			monster->SetPosition(PERVec3(x, y, 1.0));
			AddObject(monster);
		}
	}*/

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
