#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_object.h"
#include "object_pool.h"

PERWorld::PERWorld(PERObject* player, ObjectPool* objectPool)
	: m_objectPool(objectPool)
{
	m_objects.reserve(PER_DEFAULT_MAX_OBJECTS);
	
	InitWorldObject(player);

	std::cout << "Num Object: " << m_numObject << std::endl;
}

PERWorld::~PERWorld()
{
	
}

void PERWorld::Update(double dTime)
{
	DoGarbegeCollection(dTime);

	ProcessPendingMessage();
}

void PERWorld::ObjectsInputUpdate(PERController& controller, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetInput().Update(*m_objects[i], *this, controller, dTime);
	}
}

void PERWorld::ObjectsAiUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetAi().Update(*m_objects[i], *this, dTime);
	}
}

void PERWorld::ObjectsPhysicsUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetPhysics().Update(*m_objects[i], *this, dTime);
	}
}

void PERWorld::ObjectsGraphicsUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetGraphics().Update(*m_objects[i], dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer)
{
	for (int i = 0; i < m_sortedObjects.size(); ++i) {
		m_sortedObjects[i]->GetGraphics().Render(*m_sortedObjects[i], renderer);
	}
}

void PERWorld::RequestAddObject(PERObject* parent, PERObjectType type, PERVec3 position, PERVec3 currentAccel, double lifeTime)
{
	PERWorldMessage message;
	message.id = PERWorldMessageId::WORLD_MESSAGE_ADD_OBJECT;
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
	message.id = PERWorldMessageId::WORLD_MESSAGE_DELETE_OBJECT;
	message.object = object;

	if (m_maxPending == m_numPending) ResizePedingArray();
	m_pending[m_numPending] = message;
	m_numPending++;
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
		case PERWorldMessageId::WORLD_MESSAGE_ADD_OBJECT: {
			PERObject* object = AddAndGetObject(message.type);
			object->SetPosition(message.position);
			object->SetCurrentAccel(message.currentAccel);
			object->SetLifeTime(message.lifeTime);
			object->SetParent(message.object);
			break;
		}
		case PERWorldMessageId::WORLD_MESSAGE_DELETE_OBJECT: 
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

void PERWorld::InitWorldObject(PERObject* player)
{
	player->SetPosition(PERVec3(0.0, 0.0, 0.0));
	AddObject(player);

	PERObject* monster;
	for (double x = -10.0; x < 10.0; x += 0.5) {
		for (double y = -10.0; y < 10.0; y += 0.5) {
			monster = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_MONSTER);
			monster->SetPosition(PERVec3(x, y, -1.0));
			AddObject(monster);
		}
	}

	PERObject* wall;
	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(-1.0, 0.0, 1.1));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 1.2));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 1.3));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);
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
