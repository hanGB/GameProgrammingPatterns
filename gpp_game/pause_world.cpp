#include "stdafx.h"
#include "pause_world.h"
#include "menu_game_mode.h"
#include "pause_hud.h"
#include "test_game_state.h"
#include "menu_select_input_component.h"
#include "per_object.h"
#include "event_dispatcher.h"

PauseWorld::PauseWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MenuGameMode* gameMode = new MenuGameMode();
	gameMode->SetHud(new PauseHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);

	// menu별 행위 설정
	MenuSelectInputComponent* input = dynamic_cast<MenuSelectInputComponent*>(&gameMode->GetPlayer().GetInput());
	// 게임으로 돌아감
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::RESUME_GAME, PERVec3(0.0, 0.0, 0.0));
		});
	// 창 크기 변경
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::CHANGE_WINDOW_SIZE, PERVec3(0.0, 0.0, 0.0));
		});
	// 타이틀로 돌아감
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::BACK_TO_TITLE, PERVec3(0.0, 0.0, 0.0));
		});
	// 게임 종료
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::EXIT_GAME, PERVec3(0.0, 0.0, 0.0));
		});
	// ESC 눌렀을 때 게임으로 돌아감
	input->SetESCMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::RESUME_GAME, PERVec3(0.0, 0.0, 0.0));
		});
}

PauseWorld::~PauseWorld()
{
}

void PauseWorld::InitWorldObject()
{
}
