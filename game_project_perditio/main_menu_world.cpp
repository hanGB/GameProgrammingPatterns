#include "stdafx.h"
#include "main_menu_world.h"
#include "main_menu_hud.h"
#include "menu_game_mode.h"
#include "test_game_state.h"
#include "object_storage.h"

MainMenuWorld::MainMenuWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MenuGameMode* gameMode = new MenuGameMode();
	gameMode->SetHud(new MainMenuHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);
}

MainMenuWorld::~MainMenuWorld()
{

}

void MainMenuWorld::Enter(PERAudio& audio)
{
	PERWorld::Enter(audio);
}

void MainMenuWorld::Exit(PERAudio& audio)
{
	PERWorld::Exit(audio);
}

void MainMenuWorld::Pause(PERAudio& audio)
{
	PERWorld::Pause(audio);
}

void MainMenuWorld::Resume(PERAudio& audio)
{
	PERWorld::Resume(audio);
}

void MainMenuWorld::AddFixedAndPhysicalObjects()
{
	// 배경
	PERObject* background;
	background = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	background->SetPosition(PERVec3(0.0, 0.0, -1.0));
	background->SetSize(PERVec3(20.0, 20.0, 1.0));
	SetObjectShapeAndColor(background, PERShapeType::RECTANGLE, PERColor(100, 100, 100));
	AddObject(background);
}

void MainMenuWorld::AddOtherObjects()
{

}
