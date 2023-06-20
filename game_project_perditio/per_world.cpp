#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_object.h"
#include "object_pool.h"

PERWorld::PERWorld(PERObject* player, ObjectPool* objectPool)
	: m_objectPool(objectPool)
{
	player->SetPosition(PERVec3(0.0, 0.0, 0.0));
	AddObject(player);

	InitWorldObject();
}

PERWorld::~PERWorld()
{
	
}

void PERWorld::Update(PERController& controller, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) {
		m_objects[i]->GetInput().Update(*m_objects[i], controller);
	}
	for (int i = 0; i < m_numObject; ++i) {
		m_objects[i]->GetAi().Update(*m_objects[i], dTime);
	}
	for (int i = 0; i < m_numObject; ++i) {
		m_objects[i]->GetPhysics().Update(*m_objects[i], *this, dTime);
	}
	for (int i = 0; i < m_numObject; ++i) {
		m_objects[i]->GetGraphics().Update(*m_objects[i], dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer)
{
	for (int i = 0; i < m_numObject; ++i) {
		m_objects[i]->GetGraphics().Render(*m_objects[i], renderer);
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


void PERWorld::InitWorldObject()
{
	PERObject* wall;
	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(1.0, 0.0, 0.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(-1.0, 0.0, 0.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 0.0));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 0.0));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	AddObject(wall);
}
