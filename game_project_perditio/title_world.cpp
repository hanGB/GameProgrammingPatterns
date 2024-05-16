#include "stdafx.h"
#include "title_world.h"
#include "title_hud.h"
#include "menu_game_mode.h"
#include "test_game_state.h"
#include "object_storage.h"

TitleWorld::TitleWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	MenuGameMode* gameMode = new MenuGameMode();
	gameMode->SetHud(new TitleHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);
}

TitleWorld::~TitleWorld()
{

}

void TitleWorld::Enter(PERAudio& audio)
{
	PERWorld::Enter(audio);
}

void TitleWorld::Exit(PERAudio& audio)
{
	PERWorld::Exit(audio);
}

void TitleWorld::Pause(PERAudio& audio)
{
	PERWorld::Pause(audio);
}

void TitleWorld::Resume(PERAudio& audio)
{
	PERWorld::Resume(audio);
}

void TitleWorld::AddFixedAndPhysicalObjects()
{
	// 배경
	PERObject* background;
	background = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	background->SetPosition(PERVec3(0.0, 0.0, -1.0));
	background->SetSize(PERVec3(20.0, 20.0, 1.0));
	SetObjectShapeAndColor(background, PERShapeType::RECTANGLE, PERColor(150, 150, 150));
	AddObject(background);
}

void TitleWorld::AddOtherObjects()
{

}
