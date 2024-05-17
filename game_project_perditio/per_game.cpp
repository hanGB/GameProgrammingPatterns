#include "stdafx.h"
#include "per_game.h"
#include "irr_audio.h"
#include "null_audio.h"
#include "black_board.h"
#include "title_world.h"
#include "pause_world.h"
#include "test_world.h"
#include "test_world2.h"

PERGame::PERGame(HWND hWnd)
{
	PERLog::Logger().Info("게임 클래스 생성 시작");

	m_renderer = new PERRenderer(hWnd);
	m_controller = new PERController();
	m_audio = new NullAudio();
	m_objectStorage = new ObjectStorage();
	m_database = new PERDatabase();

	PERLog::Logger().Info("게임 클래스 생성 완료");
}

PERGame::~PERGame()
{
	PERLog::Logger().Info("게임 클래스 삭제 시작");

	delete m_controller;
	delete m_renderer;
	delete m_currentWorld;
	delete m_objectStorage;
	delete m_audio;

	PERLog::Logger().Info("게임 클래스 삭제 완료");
}

void PERGame::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::EXECUTE_GAME:
		PERLog::Logger().Info("게임 실행: Title");
		Run(new TitleWorld(m_objectStorage, m_database));
		break;
	case PEREvent::RUN_TEST_WORLD: 
		ProgressRunEvent<TestWorld>("Test");
		break;
	case PEREvent::RUN_TEST2_WORLD: 
		ProgressRunEvent<TestWorld2>("Test2");
		break;
	case PEREvent::CHANGE_WINDOW_SIZE:
		m_changeWindowSize = true;
		break;
	case PEREvent::EXIT_GAME:
		m_destroyWindow = true;
		break;
	case PEREvent::PAUSE_GAME:
		ProgressPauseGameEvent();
		break;
	case PEREvent::RESUME_GAME:
		ProgressResumeGameEvent();
		break;
	case PEREvent::BACK_TO_TITLE:
		ProgressQuitAllWorldAndRunEvent<TitleWorld>("Title");
		break;
	}
}

void PERGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_controller->SetKeyboardPressed(PERKeyboardValue::UP, isDown);
	if (wParam == VK_DOWN) m_controller->SetKeyboardPressed(PERKeyboardValue::DOWN, isDown);
	if (wParam == VK_LEFT) m_controller->SetKeyboardPressed(PERKeyboardValue::LEFT, isDown);
	if (wParam == VK_RIGHT) m_controller->SetKeyboardPressed(PERKeyboardValue::RIGHT, isDown);
	if (wParam == VK_SPACE) m_controller->SetKeyboardPressed(PERKeyboardValue::SPACE, isDown);
	if (wParam == VK_ESCAPE) m_controller->SetKeyboardPressed(PERKeyboardValue::ESC, isDown);

	if (wParam == 'A' || wParam == 'a') m_controller->SetKeyboardPressed(PERKeyboardValue::A, isDown);
	if (wParam == 'S' || wParam == 's') m_controller->SetKeyboardPressed(PERKeyboardValue::S, isDown);
	if (wParam == 'D' || wParam == 'd') m_controller->SetKeyboardPressed(PERKeyboardValue::D, isDown);
	if (wParam == 'F' || wParam == 'f') m_controller->SetKeyboardPressed(PERKeyboardValue::F, isDown);
	if (wParam == 'Q' || wParam == 'q') m_controller->SetKeyboardPressed(PERKeyboardValue::Q, isDown);
}

void PERGame::Update(int time)
{
	if (m_isStopWorldJob) 
	{
		m_isStopUpdate = true;
		return;
	}

	double dTime = time / 1'000'000.0;

	m_fpsUpdateTime -= dTime;
	if (m_fpsUpdateTime < 0.0) {
		m_fps = (int)(1'000'000.0 / (double)time);
		m_fpsUpdateTime = c_FPS_UPDATE_GAP;
	}
	
	m_controller->Update(dTime);

	// 죽은 오브젝트 월드에서 제거
	m_currentWorld->Update(*m_audio, dTime);

	
	// 정해진 시간만큼 업데이트가 필요한 항목 업데이트
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE 만큼씩 업데이트
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// 정해진 시간만큼 게임 업데이트
		m_currentWorld->InputUpdate(*m_controller, *m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->AiUpdate(*m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->PhysicsUpdate(*m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// 최대 업데이트 루프 횟수를 넘어서 끝날 경우를 대비해 업데이트에 걸리는 시간으로 나눔
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
	
	// 업데이트가 끝난 경우 리턴(저번 업데이트가 끝난 후 아직 렌더링이 완료되지 않아 false로 변경되지 않음)
	if (m_isUpdateEnd) return;
	m_frameGap = (double)m_updateLag / (double)PER_MICROSEC_PER_UPDATE;
	m_currentWorld->GraphicsUpdate(*m_audio, dTime);

	// 오브젝트 정렬
	m_currentWorld->UpdateSortedGraphicsComponents();
	// 카메라 업데이트
	m_currentWorld->UpdateCamera(*m_renderer, m_frameGap);

	// 업데이트 끝 완료
	m_isUpdateEnd = true;
}

void PERGame::UIUpdate(int time)
{
	if (m_isStopWorldJob)
	{
		m_isStopUIUpdate = true;
		return;
	}

	if (m_isUpdateUIEnd) return;

	int fps = m_fps;
	wsprintf(m_fpsText, L"FPS: %d", fps);
	m_currentWorld->UIUpdate(*m_controller, *m_audio, (double)time / 1'000'000.0);
	m_isUpdateUIEnd = true;
}

void PERGame::Render(HWND hWnd)
{
	m_renderFunc(*this, hWnd);
}

void PERGame::RenderCurrnetWorld(HWND hWnd)
{
	RenderWorld(hWnd, m_currentWorld);
}

void PERGame::RenderPreviousWorld(HWND hWnd)
{
	RenderWorld(hWnd, m_worldQueue.front());
}

void PERGame::RenderWorld(HWND hWnd, PERWorld* world)
{
	if (m_isStopWorldJob)
	{
		m_isStopRender = true;
		return;
	}

	// 렌더링 준비가 되지 않은 경우 리턴
	if (!m_isUpdateEnd) return;

	if (!m_isRenderEnd) {
		m_renderer->ResetMemoryDC(hWnd);

		// 게임 월드 렌더링
		world->Render(*m_renderer, m_frameGap);

		// 네비게이션 데이터 테스트용
		//BlackBoard::GetNavigationData().RenderOutData(*m_renderer);

		m_isRenderEnd = true;
	}

	if (!m_isRenderUIEnd) return;

	m_renderer->FillHDCWithMemoryDCs(hWnd);

	m_isRenderUIEnd = false;
	m_isUpdateEnd = false;
	m_isRenderEnd = false;
}

void PERGame::UIRender(HWND hWnd)
{
	if (m_isStopWorldJob)
	{
		m_isStopUIRender = true;
		return;
	}

	if (!m_isUpdateUIEnd || m_isRenderUIEnd) return;

	m_renderer->ResetUIMemoryDC(hWnd);
	m_renderer->RenderShapeInScreenCoordinate(PERShapeType::RECTANGLE,
		PERVec2(-0.95, -0.95), PERVec2(0.2, 0.1), PERColor(255, 255, 255), false);
	m_renderer->RenderFontInScreenCoordinate(m_fpsText, 8, 0.001, PERVec2(-0.97, -0.9), PERColor(0, 0, 0));
	m_currentWorld->UIRender(*m_renderer);

	m_isRenderUIEnd = true;
	m_isUpdateUIEnd = false;
}

void PERGame::AudioUpdate()
{
	m_audio->Update();
}

void PERGame::MatchWindowHWND(HWND hWnd)
{
	m_renderer->MatchWindowSize(hWnd);
}

void PERGame::GetWindowSize(int* width, int* height)
{
	*width = m_windowSizeW;
	*height = m_windowSizeH;
}

void PERGame::ChangeWindowSize(HWND hWnd)
{
	if (m_isMaxScreenSize)
	{
		m_windowSizeW = PER_DEFAULT_WINDOW_WIDTH;
		m_windowSizeH = PER_DEFAULT_WINDOW_HEIGHT;
		m_isMaxScreenSize = false;
		SetWindowPos(hWnd, nullptr, PER_DEFAULT_WINDOW_LOCATION_X, PER_DEFAULT_WINDOW_LOCATION_Y, m_windowSizeW, m_windowSizeH, 0);
	}
	else {
		m_windowSizeW = PER_MAXIMUM_WINDOW_WIDTH;
		m_windowSizeH = PER_MAXIMUM_WINDOW_HEIGHT;
		m_isMaxScreenSize = true;
		SetWindowPos(hWnd, nullptr, 0, 0, m_windowSizeW, m_windowSizeH, 0);
	}
}

void PERGame::DoWindowJob(HWND hWnd)
{
	if (m_changeWindowSize)
	{
		ChangeWindowSize(hWnd);
		MatchWindowHWND(hWnd);
		m_changeWindowSize = false;
	}
	if (m_destroyWindow)
	{
		PostMessageW(hWnd, WM_DESTROY, 0, 0);
		m_destroyWindow = false;
	}
}
void PERGame::ProgressPopEvent()
{
	PERLog::Logger().InfoWithFormat("기존 월드를 삭제하고 큐에 있는 월드를 꺼냄");

	StopWorldJob();

	GivePlayStateToNextWorld(m_worldQueue.front());
	Quit();
	PopWorld();

	RestartWorldJob();
}

void PERGame::ProgressPauseGameEvent()
{
	StopWorldJob();

	PERWorld* world = new PauseWorld(m_objectStorage, m_database);
	GivePlayStateToNextWorld(world);
	PushWorld();
	// 이전 월드의 오브젝트들을 렌더링 하도록 설정
	m_renderFunc = &PERGame::RenderPreviousWorld;
	Run(world);

	RestartWorldJob();
}

void PERGame::ProgressResumeGameEvent()
{
	StopWorldJob();

	GivePlayStateToNextWorld(m_worldQueue.front());
	Quit();
	// 현재 월드의 오브젝트들을 렌더링 하도록 되돌림
	m_renderFunc = &PERGame::RenderCurrnetWorld;
	PopWorld();

	RestartWorldJob();
}

void PERGame::Run(PERWorld* world)
{
	PERLog::Logger().Info("월드 실행");

	world->Enter(*m_renderer, *m_audio);

	m_currentWorld = world;
}

void PERGame::PushWorld()
{
	PERLog::Logger().Info("기존 월드 저장");

	m_currentWorld->Pause(*m_renderer, *m_audio);

	m_worldQueue.push(m_currentWorld);
}

void PERGame::PopWorld()
{
	PERLog::Logger().Info("저장된 기존 월드를 꺼냄");

	PERWorld* world = m_worldQueue.front();
	m_worldQueue.pop();

	world->Resume(*m_renderer, *m_audio);

	m_currentWorld = world;
}

void PERGame::Quit()
{
	if (!m_currentWorld) return;

	PERLog::Logger().Info("현재 월드 종료");

	m_currentWorld->Exit(*m_renderer, *m_audio);

	delete m_currentWorld;
}

void PERGame::QuitAllWorld()
{
	// 현재 월드 종료
	Quit();
	while (!m_worldQueue.empty()) {
		PERWorld* world = m_worldQueue.front();
		world->Exit(*m_renderer, *m_audio);
		delete world;
		m_worldQueue.pop();
	}
	// 재설정
	m_renderFunc = &PERGame::RenderCurrnetWorld;
}

void PERGame::StopWorldJob()
{
	m_isStopWorldJob = true;

	// 모두 종료될 때까지 대기
	while (true) {
		Sleep(1);
		if (m_isStopUpdate && m_isStopUIUpdate && m_isStopRender && m_isStopUIRender) break;
	}
}

void PERGame::RestartWorldJob()
{
	m_isStopWorldJob = false;
	m_isStopUpdate = false;
	m_isStopUIUpdate = false;
	m_isStopRender = false;
	m_isStopUIRender = false;
}

void PERGame::GivePlayStateToNextWorld(PERWorld* nextWorld)
{
	if (m_currentWorld)
	{
		PlayerState* playerState = &m_currentWorld->GetGameMode().GetPlayerState();
		nextWorld->GetGameMode().UpdatePlayerState(playerState);
	}
}

