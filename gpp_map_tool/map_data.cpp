#include "map_tool_world.h"
#include "object_storage.h"
#include "per_database.h"

void MapToolWorld::TestFixedObjects()
{
	// 플랫폼
	PERObject* platform;
	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(platform, "EARTHEN_GROUND_VISUAL");
	platform->SetPosition(PERVec3(0.0, 0.0, -1.0));
	platform->SetSize(PERVec3(15.0, 20.0, 1.0));
	platform->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(platform);

	for (double x = 0.0; x <= 20.0; x += 20.0) {
		platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
		SetObjectVisual(platform, "EARTHEN_GROUND_VISUAL");
		platform->SetPosition(PERVec3(-10.0 + x, 0.0, -1.0));
		platform->SetSize(PERVec3(5.0, 5.0, 1.0));
		platform->SetMass(PER_FIXED_OBJECT_MASS);
		AddObject(platform);
	}

	// 지붕
	PERObject* roof;
	roof = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(roof, "BLUE_ROOF_VISUAL");
	roof->SetPosition(PERVec3(1.0, 0.0, 1.0));
	roof->SetSize(PERVec3(1.0, 2.0, 1.0));
	roof->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(roof);

	roof = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(roof, "BLUE_ROOF_VISUAL");
	roof->SetPosition(PERVec3(-1.0, 0.0, 1.0));
	roof->SetSize(PERVec3(1.0, 2.0, 1.0));
	roof->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(roof);
	
	roof = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(roof, "BLUE_ROOF_VISUAL");
	roof->SetPosition(PERVec3(0.0, 1.0, 1.1));
	roof->SetSize(PERVec3(2.0, 1.0, 1.0));
	roof->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(roof);

	roof = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(roof, "BLUE_ROOF_VISUAL");
	roof->SetPosition(PERVec3(0.0, -1.0, 1.1));
	roof->SetSize(PERVec3(2.0, 1.0, 1.0));
	roof->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(roof);
	
	// 벽
	PERObject* wall;
	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(-5.0, 0.0, 0.1));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);
}

void MapToolWorld::Test2FixedObjects()
{
	// 플랫폼
	PERObject* platform;
	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(platform, "EARTHEN_GROUND_VISUAL");
	platform->SetPosition(PERVec3(0.0, 0.0, -1.0));
	platform->SetSize(PERVec3(10.0, 10.0, 1.0));
	platform->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(platform);

	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(platform, "EARTHEN_GROUND_VISUAL");
	platform->SetPosition(PERVec3(-7.5, 0.0, -1.0));
	platform->SetSize(PERVec3(5.0, 5.0, 1.0));
	platform->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(platform);

	// 벽
	PERObject* wall;
	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 10.0, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(-5.0, 3.0, 0.1));
	wall->SetSize(PERVec3(0.5, 4.0, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(-5.0, -3.0, 0.1));
	wall->SetSize(PERVec3(0.5, 4.0, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(10.0, 0.5, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(wall, "WOODEN_WALL_VISUAL");
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(10.0, 0.5, 1.0));
	wall->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(wall);
}

void MapToolWorld::TitleFixedObjects()
{
	// 배경
	PERObject* background;
	background = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(background, "TITLE_BACKGOUND");
	background->SetPosition(PERVec3(0.0, 0.0, -1.0));
	background->SetSize(PERVec3(20.0, 20.0, 1.0));
	background->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(background);
}
