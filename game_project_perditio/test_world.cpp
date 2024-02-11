#include "stdafx.h"
#include "test_world.h"
#include "object_spawner.h"
#include "per_object.h"
#include "object_pool.h"
#include "game_mode.h"

TestWorld::TestWorld(ObjectPool* objectPool, GameMode* mode)
{
	InitSettingForWorld(objectPool, mode);
}

TestWorld::~TestWorld()
{
	DeleteWorldObjects();
}

void TestWorld::Enter()
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 0.0, 0.0));
	m_gameMode->GetPlayer().SetCurrentPositionToSpawnPosition();

	PERWorld::Enter();
}

void TestWorld::Exit()
{
	PERWorld::Exit();
}

void TestWorld::Pause()
{
	PERWorld::Pause();
}

void TestWorld::Resume()
{
	PERWorld::Resume();
}

void TestWorld::AddFixedAndPhysicalObjects()
{
	// ÇÃ·§Æû
	PERObject* platform;
	platform = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	platform->SetPosition(PERVec3(0.0, 0.0, -1.0));
	platform->SetSize(PERVec3(15.0, 20.0, 1.0));
	SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
	AddObject(platform);

	for (double x = 0.0; x <= 20.0; x += 20.0) {
		platform = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
		platform->SetPosition(PERVec3(-10.0 + x, 0.0, -1.0));
		platform->SetSize(PERVec3(5.0, 5.0, 1.0));
		SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
		AddObject(platform);
	}

	// º®
	PERObject* wall;
	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 1.1));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 1.1));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-5.0, 0.0, 0.1));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectPool->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);
}

void TestWorld::AddOtherObjects()
{
	m_monsterSpawners = new ObjectSpawner[4];
	int monster = 0;
	for (double x = -3.0; x <= 3.0; x += 6.0) {
		for (double y = -3.0; y <= 3.0; y += 6.0) {
			PERStat stat = { 1, 50, 50, 5, 5, 5, 5 };
			m_monsterSpawners[monster].SetSpawner(PERObjectType::MONSTER, stat, PERVec3(x, y, 0.0));
			monster++;
		}
	}

	PERObject* block;
	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(3.0, 1.0, 0.0));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-3.0, 1.0, 0.0));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(1.0, 3.0, 0.0));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	block = m_objectPool->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(1.0, -3.0, 0.0));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);
}

void TestWorld::DeleteWorldObjects()
{
	delete[] m_monsterSpawners;
}

void TestWorld::WorldUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < 4; ++i) {
		PERObject* monster = m_monsterSpawners[i].SpawnWithLiving(*m_objectPool);
		if (monster) AddObject(monster);
	}
}
