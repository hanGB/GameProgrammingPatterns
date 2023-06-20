#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_object.h"
#include "object_pool.h"

PERWorld::PERWorld(PERObject* player, ObjectPool* objectPool)
	: m_player(player), m_objectPool(objectPool)
{
	m_player->SetPosition(PERVec3(0.0, 0.0, 0.0));
	InitWorldObject();
}

PERWorld::~PERWorld()
{
	delete m_wall1;
	delete m_wall2;
}

void PERWorld::Update(PERController& controller, double dTime)
{
	m_wall1->Update(controller, *this, dTime);
	m_wall2->Update(controller, *this, dTime);
	m_player->Update(controller, *this, dTime);
}

void PERWorld::Render(PERRenderer& renderer)
{
	m_wall1->Render(renderer);
	m_wall2->Render(renderer);
	m_player->Render(renderer);
}

void PERWorld::InitWorldObject()
{
	m_wall1 = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	m_wall1->SetPosition(PERVec3(1.0, 0.0, 0.0));
	m_wall1->SetSize(PERVec3(1.0, 2.0, 1.0));

	m_wall2 = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_BLOCK);
	m_wall2->SetPosition(PERVec3(-1.0, 0.0, 0.0));
	m_wall2->SetSize(PERVec3(1.0, 2.0, 1.0));
}
