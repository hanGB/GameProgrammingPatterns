#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_object.h"
#include "object_factory.h"

PERWorld::PERWorld(PERObject* player, ObjectFactory* objectFactory)
	: m_player(player), m_objectFactory(objectFactory)
{
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
	m_wall1 = m_objectFactory->CreateWall();
	m_wall1->SetPosition(PERVec3(1.0, 0.0, 0.0));
	m_wall1->SetSize(PERVec2(1.0, 2.0));

	m_wall2 = m_objectFactory->CreateWall();
	m_wall2->SetPosition(PERVec3(-1.0, 0.0, 0.0));
	m_wall2->SetSize(PERVec2(1.0, 2.0));
}
