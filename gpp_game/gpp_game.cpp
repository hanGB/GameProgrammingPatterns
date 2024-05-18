#include "gpp_game.h"
#include "title_world.h"
#include "pause_world.h"
#include "credits_world.h"
#include "test_world.h"
#include "test_world2.h"

GPPGame::GPPGame(HWND hWnd)
{
	Initialize(hWnd);
}

GPPGame::~GPPGame()
{

}

void GPPGame::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::EXECUTE_GAME:
		PERLog::Logger().Info("게임 실행: Title");
		Run(MakeWorld<TitleWorld>());
		break;
	case PEREvent::RUN_TEST_WORLD:
		ProgressRunEvent<TestWorld>("Test");
		break;
	case PEREvent::RUN_TEST2_WORLD:
		ProgressRunEvent<TestWorld2>("Test2");
		break;
	case PEREvent::CHANGE_WINDOW_SIZE:
		ChangeWindowSize();
		break;
	case PEREvent::EXIT_GAME:
		DestroyWindow();
		break;
	case PEREvent::PAUSE_GAME:
		ProgressPauseGameEvent<PauseWorld>("Pause");
		break;
	case PEREvent::RESUME_GAME:
		ProgressResumeGameEvent();
		break;
	case PEREvent::BACK_TO_TITLE:
		ProgressQuitAllWorldAndRunEvent<TitleWorld>("Title");
		break;
	case PEREvent::GO_TO_CREDITS:
		ProgressRunEvent<CreditsWorld>("Credits");
		break;
	}
}
