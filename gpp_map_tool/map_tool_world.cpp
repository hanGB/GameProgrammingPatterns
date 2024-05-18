#include "map_tool_world.h"
#include "map_tool_game_mode.h"
#include "map_tool_hud.h"
#include "game_state.h"
#include "object_storage.h"
#include "per_database.h"

MapToolWorld::MapToolWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MapToolGameMode* gameMode = new MapToolGameMode();
	gameMode->SetHud(new MapToolHud());
	gameMode->SetGameState(new GameState());

	InitSettingForWorld(objectStorage, database, gameMode);
}

MapToolWorld::~MapToolWorld()
{
}

void MapToolWorld::Enter(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 0.0, 0.1));

	PERWorld::Enter(renderer, audio);
}


void MapToolWorld::AddFixedAndPhysicalObjects()
{
	//ReadFixedObjectsDataFromFile("./map/test_fixed.map");
	TitleFixedObjects();
	MakeFixedObjectsDataToFile("title_fixed.map");
}

void MapToolWorld::AddOtherObjects()
{
}


