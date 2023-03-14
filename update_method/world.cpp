#include "stdafx.h"
#include "world.h"
#include "monster.h"

void World::GameLoop()
{
	while (true) {
		UpdateEntitiesList();

		// user inputs

		// update
		for (int i = 0; i < m_numEntities; ++i) {
			m_entities[i]->Update(0.016);
		}

		// physics

		// render
		RenderWall();
		for (int i = 0; i < m_numEntities; ++i) {
			m_entities[i]->Render(m_renderer);
		}
		m_renderer->FlipBuffer();

		Sleep(16);
	}
}

void World::AddEntityAfterOneLoopEnd(Entity* entity)
{
	m_tempEntities[m_tempNumEntities] = entity;
	m_tempNumEntities++;
}

void World::Init()
{
	Skeleton* skeleton1 = new Skeleton();
	skeleton1->SetModel('S');
	skeleton1->SetX(0.0);
	skeleton1->SetY(3.0);
	m_entities[m_numEntities] = skeleton1; 
	m_numEntities++;

	Skeleton* skeleton2 = new Skeleton();
	skeleton2->SetModel('s');
	skeleton2->SetX(-5.0);
	skeleton2->SetY(-5.0);
	m_entities[m_numEntities] = skeleton2;
	m_numEntities++;

	Skeleton* skeleton3 = new Skeleton();
	skeleton3->SetModel('s');
	skeleton3->SetX(5.0);
	skeleton3->SetY(-1.0);
	m_entities[m_numEntities] = skeleton3;
	m_numEntities++;


	Statue* statue1 = new Statue(3.0);
	statue1->SetModel('T');
	statue1->SetX(5);
	statue1->SetY(5.0);
	statue1->SetWorld(this);
	m_entities[m_numEntities] = statue1;
	m_numEntities++;

	Statue* statue2 = new Statue(3.0);
	statue2->SetModel('T');
	statue2->SetX(-5.0);
	statue2->SetY(5.0);
	statue2->SetWorld(this);
	m_entities[m_numEntities] = statue2;
	m_numEntities++;
}

void World::UpdateEntitiesList()
{
	for (int i = 0; i < m_numEntities; ++i) {
		if (m_entities[i]->GetIsDead()) {
			m_entities[i] = nullptr;
			Entity* temp = m_entities[i];
			delete temp;

			m_numEntities--;
			for (int t = i; t < m_numEntities; ++t) {
				m_entities[t] = m_entities[t + 1];
			}
		}
	}

	for (int i = 0; i < m_tempNumEntities; ++i) {
		m_entities[m_numEntities] = m_tempEntities[i];
		m_numEntities++;
	}
	m_tempNumEntities = 0;
}

void World::DeleteAllEntities()
{
	UpdateEntitiesList();

	for (int i = 0; i < m_numEntities; ++i) {
		m_entities[i] = nullptr;
		Entity* temp = m_entities[i];
		delete temp;
	}
	m_numEntities = 0;
}

void World::RenderWall()
{
	int y = WORLD_HEIGHT;
	for (int x = -WORLD_WIDTH; x <= WORLD_WIDTH; ++x) {
		m_renderer->PrintOnBuffer(x, y, "#");
	}
	for (y; y > (-WORLD_HEIGHT); --y) {
		m_renderer->PrintOnBuffer(-WORLD_WIDTH, y, "#");
		m_renderer->PrintOnBuffer(WORLD_WIDTH, y, "#");
	}
	for (int x = -WORLD_WIDTH; x <= WORLD_WIDTH; ++x) {
		m_renderer->PrintOnBuffer(x, y, "#");
	}
}

