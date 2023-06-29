#include "stdafx.h"
#include "per_game.h"

PERGame::PERGame(HWND hWnd)
{
	m_controller = new PERController();
	m_renderer = new PERRenderer(hWnd);
	m_objectPool = new ObjectPool();

	m_player = m_objectPool->PopObject(PERObjectType::OBJECT_TYPE_PLAYER);

	m_world = new PERWorld(m_player, m_objectPool);
}

PERGame::~PERGame()
{
	delete m_controller;
	delete m_renderer;
	delete m_world;
	delete m_objectPool;
	delete m_player;
}

void PERGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_UP, isDown);
	if (wParam == VK_DOWN) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN, isDown);
	if (wParam == VK_LEFT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT, isDown);
	if (wParam == VK_RIGHT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT, isDown);
	if (wParam == VK_SPACE) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_SPACE, isDown);

	if (wParam == 'A' || wParam == 'a') m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_A, isDown);
	if (wParam == 'S' || wParam == 's') m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_S, isDown);
	if (wParam == 'D' || wParam == 'd') m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_D, isDown);
	if (wParam == 'F' || wParam == 'f') m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_F, isDown);
}

void PERGame::Update(int time)
{
	int restTime = PER_MINIMUM_FRAME_TIME - time;

	if (restTime > 0) {
		std::this_thread::sleep_for(std::chrono::microseconds(restTime));
		time = PER_MINIMUM_FRAME_TIME;
	}
	double dTime = time / 1'000'000.0;

	m_fpsUpdateTime -= dTime;
	if (m_fpsUpdateTime < 0.0) {
		m_fps = (int)(1'000'000.0 / (double)time);
		m_fpsUpdateTime = c_FPS_UPDATE_GAP;
	}
	
	
	m_controller->Update(dTime);

	// 죽은 오브젝트 월드에서 제거
	m_world->Update(dTime);

	
	// 정해진 시간만큼 업데이트가 필요한 항목 업데이트
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE 만큼씩 업데이트
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// 정해진 시간만큼 게임 업데이트
		m_world->ObjectsInputUpdate(*m_controller, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_world->ObjectsAiUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_world->ObjectsPhysicsUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// 최대 업데이트 루프 횟수를 넘어서 끝날 경우를 대비해 업데이트에 걸리는 시간으로 나눔
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
	
	// 렌더링 준비가 되었을 경우 리턴(아직 렌더링 하지 않음) 
	if (m_isReadyForRender) return;

	m_world->UIUpdate(dTime);

	m_frameGap = (double)m_updateLag / (double)PER_MICROSEC_PER_UPDATE;
	m_world->ObjectsGraphicsUpdate(dTime); 

	m_world->UpdateSortedObjects();
	wsprintf(m_fpsText, L"FPS: %d", m_fps);

	// 렌더링 준비 완료
	m_isReadyForRender = true;
}

void PERGame::Render(HWND hWnd)
{
	// 렌더링 준비가 되지 않은 경우 리턴
	if (!m_isReadyForRender) return;

	// 윈도우 크기 설정
	m_renderer->MatchWindowSize(hWnd);

	m_renderer->ResetMemoryDC(hWnd);
	m_renderer->ResetUIMemoryDC(hWnd);

	// 게임 월드 렌더링
	m_world->Render(*m_renderer, m_frameGap);
	m_renderer->RenderShapeInScreenCoordinate(PERShapeType::SHAPE_TYPE_RECTANGLE,
		PERVec2(-0.95, -0.95), PERVec2(0.2, 0.1), PERColor(255, 255, 255), false);
	m_renderer->RenderFontInScreenCoordinate(m_fpsText, 8, 0.001, PERVec2(-0.97, -0.9), PERColor(0, 0, 0));
	m_world->UIRender(*m_renderer);

	m_renderer->FillHDCWithMemoryDCs(hWnd);

	// 렌더링 준비를 하도록 변경
	m_isReadyForRender = false;
}

PERController& PERGame::GetController()
{
	return *m_controller;
}

PERRenderer& PERGame::GetRenderer()
{
	return *m_renderer;
}

