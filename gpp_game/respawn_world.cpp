#include "stdafx.h"
#include "respawn_world.h"
#include "menu_game_mode.h"
#include "respawn_hud.h"
#include "test_game_state.h"
#include "menu_select_input_component.h"
#include "per_object.h"
#include "event_dispatcher.h"

RespawnWorld::RespawnWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MenuGameMode* gameMode = new MenuGameMode();
	gameMode->SetHud(new RespawnHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);

	// menu별 행위 설정
	MenuSelectInputComponent* input = dynamic_cast<MenuSelectInputComponent*>(&gameMode->GetPlayer().GetInput());
	// 리스폰
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::RESTART_GAME, PERVec3(0.0, 0.0, 0.0));
		});
	// 타이틀로
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::BACK_TO_TITLE, PERVec3(0.0, 0.0, 0.0));
		});
	// ESC 눌렀을 때 게임 종료
	input->SetESCMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::EXIT_GAME, PERVec3(0.0, 0.0, 0.0));
		});
}

RespawnWorld::~RespawnWorld()
{
}

void RespawnWorld::InitWorldObject()
{
}