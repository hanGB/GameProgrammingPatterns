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

	// ���� ������Ʈ ���忡�� ����
	m_world->Update(dTime);

	
	// ������ �ð���ŭ ������Ʈ�� �ʿ��� �׸� ������Ʈ
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE ��ŭ�� ������Ʈ
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// ������ �ð���ŭ ���� ������Ʈ
		m_world->ObjectsInputUpdate(*m_controller, PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_world->ObjectsAiUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_world->ObjectsPhysicsUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// �ִ� ������Ʈ ���� Ƚ���� �Ѿ ���� ��츦 ����� ������Ʈ�� �ɸ��� �ð����� ����
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
	
	// ������Ʈ�� ���� ��� ����
	if (m_isUpdateEnd) return;
	m_frameGap = (double)m_updateLag / (double)PER_MICROSEC_PER_UPDATE;
	m_world->ObjectsGraphicsUpdate(dTime); 

	m_world->UpdateSortedObjects();

	// ������Ʈ �� �Ϸ�
	m_isUpdateEnd = true;
}

void PERGame::UIUpdate(int time)
{
	if (m_isUpdateUIEnd) return;

	int fps = m_fps;
	wsprintf(m_fpsText, L"FPS: %d", fps);
	m_world->UIUpdate((double)time / 1'000'000.0);
	m_isUpdateUIEnd = true;
}

void PERGame::Render(HWND hWnd)
{
	// ������ �غ� ���� ���� ��� ����
	if (!m_isUpdateEnd) return;

	if (!m_isRenderEnd) {
		m_renderer->ResetMemoryDC(hWnd);

		// ���� ���� ������
		m_world->Render(*m_renderer, m_frameGap);
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
	m_renderer->RenderShapeInScreenCoordinate(PERShapeType::SHAPE_TYPE_RECTANGLE,
		PERVec2(-0.95, -0.95), PERVec2(0.2, 0.1), PERColor(255, 255, 255), false);
	m_renderer->RenderFontInScreenCoordinate(m_fpsText, 8, 0.001, PERVec2(-0.97, -0.9), PERColor(0, 0, 0));
	m_world->UIRender(*m_renderer);

	m_isRenderUIEnd = true;
	m_isUpdateUIEnd = false;
}

PERController& PERGame::GetController()
{
	return *m_controller;
}

PERRenderer& PERGame::GetRenderer()
{
	return *m_renderer;
}

