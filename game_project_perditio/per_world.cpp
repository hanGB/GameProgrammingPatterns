#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_object.h"
#include "object_pool.h"

#include "per_graphics_component.h"

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

void PERWorld::InputUpdate(PERController& controller, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetInput().Update(*m_objects[i], controller);
	}
}

void PERWorld::AiUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetAi().Update(*m_objects[i], dTime);
	}
}

void PERWorld::PhysicsUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetPhysics().Update(*m_objects[i], *this, dTime);
	}
}

void PERWorld::GraphicsUpdate(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_objects[i]->GetGraphics().Update(*m_objects[i], dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer)
{
	// 위치의 z값이 작은 순서로 정렬
	std::vector<PERObject*> sortedObjects;
	sortedObjects.resize((size_t)m_numObject);
	std::copy(m_objects.begin(), m_objects.begin() + m_numObject, sortedObjects.begin());
	std::sort(sortedObjects.begin(), sortedObjects.end(), [](PERObject* a, PERObject* b) {
		return a->GetPosition().z < b->GetPosition().z;
	});

	for (int i = 0; i < m_numObject; ++i) {
		sortedObjects[i]->GetGraphics().Render(*sortedObjects[i], renderer);
	}
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
}

void PERWorld::DeleteObject(int id)
{
	m_numObject--;
	PERObject* object = m_objects[id];
	m_objects[id] = m_objects[m_numObject];
	m_objects[id]->SetIDInWorld(id);
	m_objectPool->PushObject(object->GetObjectType(), object);
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
	wall->SetPosition(PERVec3(-1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 1.0));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 1.0));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);
}
