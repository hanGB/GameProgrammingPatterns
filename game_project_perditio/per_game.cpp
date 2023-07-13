#include "stdafx.h"
#include "per_game.h"
#include "null_audio.h"

PERGame::PERGame(HWND hWnd)
{
	PERLog::Logger().Info("���� Ŭ���� ���� ����");

	m_renderer = new PERRenderer(hWnd);
	m_controller = new PERController();
	m_audio = new NullAudio();
	m_objectPool = new ObjectPool();

	PERLog::Logger().Info("���� Ŭ���� ���� �Ϸ�");
}

PERGame::~PERGame()
{
	PERLog::Logger().Info("���� Ŭ���� ���� ����");

	delete m_controller;
	delete m_renderer;
	delete m_currentWorld;
	delete m_objectPool;
	delete m_player;

	PERLog::Logger().Info("���� Ŭ���� ���� �Ϸ�");
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

	// ���� ������Ʈ ���忡�� ����
	m_currentWorld->Update(m_audio, dTime);

	
	// ������ �ð���ŭ ������Ʈ�� �ʿ��� �׸� ������Ʈ
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE ��ŭ�� ������Ʈ
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// ������ �ð���ŭ ���� ������Ʈ
		m_currentWorld->ObjectsInputUpdate(*m_controller, m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->ObjectsAiUpdate(m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_currentWorld->ObjectsPhysicsUpdate(m_audio, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// �ִ� ������Ʈ ���� Ƚ���� �Ѿ ���� ��츦 ����� ������Ʈ�� �ɸ��� �ð����� ����
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
	
	// ������Ʈ�� ���� ��� ����
	if (m_isUpdateEnd) return;
	m_frameGap = (double)m_updateLag / (double)PER_MICROSEC_PER_UPDATE;
	m_currentWorld->ObjectsGraphicsUpdate(m_audio, dTime);

	m_currentWorld->UpdateSortedObjects();

	// ������Ʈ �� �Ϸ�
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
	// ������ �غ� ���� ���� ��� ����
	if (!m_isUpdateEnd) return;

	if (!m_isRenderEnd) {
		m_renderer->ResetMemoryDC(hWnd);

		// ���� ���� ������
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
	PERLog::Logger().Info("���� ���� ����");

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

