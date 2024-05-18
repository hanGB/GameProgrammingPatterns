#include "stdafx.h"
#include "title_world.h"
#include "title_hud.h"
#include "menu_game_mode.h"
#include "test_game_state.h"
#include "object_storage.h"
#include "menu_select_input_component.h"
#include "event_dispatcher.h"

TitleWorld::TitleWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MenuGameMode* gameMode = new MenuGameMode();
	gameMode->SetHud(new TitleHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);

	// menu별 행위 설정
	MenuSelectInputComponent* input = dynamic_cast<MenuSelectInputComponent*>(&gameMode->GetPlayer().GetInput());
	// 게임 시작
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::RUN_TEST_WORLD, PERVec3(0.0, 0.0, 0.0));
		});
	// 창 크기 변경
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::CHANGE_WINDOW_SIZE, PERVec3(0.0, 0.0, 0.0));
		});
	// 게임 종료
	input->AddMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::EXIT_GAME, PERVec3(0.0, 0.0, 0.0));
		});
	// ESC 눌렀을 때 게임 종료
	input->SetESCMenu([](PERWorld&, PERAudio&, double) {
		EventDispatcher::Send(PEREvent::EXIT_GAME, PERVec3(0.0, 0.0, 0.0));
		});
}

TitleWorld::~TitleWorld() 
{

}

void TitleWorld::Enter(PERRenderer& renderer, PERAudio& audio)
{
	renderer.SetCameraPosition(PERVec2(0.0, 0.0));
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 0.0, 0.1));

	PERWorld::Enter(renderer, audio);
}

void TitleWorld::Exit(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Exit(renderer, audio);
}

void TitleWorld::Pause(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Pause(renderer, audio);
}

void TitleWorld::Resume(PERRenderer& renderer, PERAudio& audio)
{
	renderer.SetCameraPosition(PERVec2(0.0, 0.0));

	PERWorld::Resume(renderer, audio);
}

void TitleWorld::InitWorldObject()
{
	MakeBackground();
}

void TitleWorld::MakeBackground()
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

