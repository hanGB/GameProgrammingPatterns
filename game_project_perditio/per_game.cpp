#include "stdafx.h"
#include "per_game.h"
#include "null_audio.h"

PERGame::PERGame(HWND hWnd)
{
	PERLog::Logger().Info("게임 클래스 생성 시작");

	m_renderer = new PERRenderer(hWnd);
	m_controller = new PERController();
	m_audio = new NullAudio();
	m_objectPool = new ObjectPool();

	PERLog::Logger().Info("게임 클래스 생성 완료");
}

PERGame::~PERGame()
{
	PERLog::Logger().Info("게임 클래스 삭제 시작");

	delete m_controller;
	delete m_renderer;
	delete m_currentWorld;
	delete m_objectPool;
	delete m_player;

	PERLog::Logger().Info("게임 클래스 삭제 완료");
}

void PERGame::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::RUN_DEFAULT_WORLD_AND_GAME_MODE: {
		GameMode* gameMode = new GameMode();
		PERWorld* world = new PERWorld(m_objectPool, gameMode);
		Run(world, gameMode);
		break; 
	}

	}
}

void PERGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_controller->SetKeyboardPressed(PERKeyboardValue::UP, isDown);
	if (wParam == VK_DOWN) m_controller->SetKeyboardPressed(PERKeyboardValue::DOWN, isDown);
	if (wParam == VK_LEFT) m_controller->SetKeyboardPressed(PERKeyboardValue::LEFT, isDown);
	if (wParam == VK_RIGHT) m_controller->SetKeyboardPressed(PERKeyboardValue::RIGHT, isDown);
	if (wParam == VK_SPACE) m_controller->SetKeyboardPressed(PERKeyboardValue::SPACE, isDown);

	if (wParam == 'A' || wParam == 'a') m_controller->SetKeyboardPressed(PERKeyboardValue::A, isDown);
	if (wParam == 'S' || wParam == 's') m_controller->SetKeyboardPressed(PERKeyboardValue::S, isDown);
	if (wParam == 'D' || wParam == 'd') m_controller->SetKeyboardPressed(PERKeyboardValue::D, isDown);
	if (wParam == 'F' || wParam == 'f') m_controller->SetKeyboardPressed(PERKeyboardValue::F, isDown);
}

void PERGame::Update(int time)
{
	double dTime = time / 1'000'000.0;

	m_fpsUpdateTime -= dTime;
	if (m_fpsUpdateTime < 0.0) {
		m_fps = (int)(1'000'000.0 / (double)time);
		m_fpsUpdateTime = c_FPS_UPDATE_GAP;
	}
	
	m_controller->Update(dTime);

	// 죽은 오브젝트 월드에서 제거
	m_currentWorld->Update(m_audio, dTime);

	
	// 정해진 시간만큼 업데이트가 필요한 항목 업데이트
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE 만큼씩 업데이트
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// 정해진 시간만큼 게임 업데이트
		m_currentWorld->ObjectsInputUpdate(*m_controller, m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->ObjectsAiUpdate(m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->ObjectsPhysicsUpdate(m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// 최대 업데이트 루프 횟수를 넘어서 끝날 경우를 대비해 업데이트에 걸리는 시간으로 나눔
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
	
	// 업데이트가 끝난 경우 리턴
	if (m_isUpdateEnd) return;
	m_frameGap = (double)m_updateLag / (double)PER_MICROSEC_PER_UPDATE;
	m_currentWorld->ObjectsGraphicsUpdate(m_audio, dTime);

	m_currentWorld->UpdateSortedObjects();

	// 업데이트 끝 완료
	m_isUpdateEnd = true;
}

void PERGame::UIUpdate(int time)
{
	if (m_isUpdateUIEnd) return;

	int fps = m_fps;
	wsprintf(m_fpsText, L"FPS: %d", fps);
	m_currentWorld->UIUpdate(m_audio, (double)time / 1'000'000.0);
	m_isUpdateUIEnd = true;
}

void PERGame::Render(HWND hWnd)
{
	// 렌더링 준비가 되지 않은 경우 리턴
	if (!m_isUpdateEnd) return;

	if (!m_isRenderEnd) {
		m_renderer->ResetMemoryDC(hWnd);

		// 게임 월드 렌더링
		m_currentWorld->Render(*m_renderer, m_frameGap);
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

void PERGame::Run(PERWorld* world, GameMode* gameMode)
{
	PERLog::Logger().Info("월드 최초 실행");

	world->Enter();

	m_currentWorld = world;
	m_currentGameMode = gameMode;
}

void PERGame::ChangeWorld(PERWorld* world, GameMode* gameMode)
{

}

void PERGame::PushWorld(PERWorld* world, GameMode* gameMode)
{
}

void PERGame::PopWorld()
{
}

void PERGame::Quit()
{
}

