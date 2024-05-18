#include "map_tool_game.h"
#include "map_tool_world.h"

MapToolGame::MapToolGame(HWND hWnd)
{
	Initialize(hWnd);
}

MapToolGame::~MapToolGame()
{

}

void MapToolGame::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::EXECUTE_GAME:
		PERLog::Logger().Info("게임 실행: Title");
		Run(MakeWorld<MapToolWorld>());
		break;
	}
}
