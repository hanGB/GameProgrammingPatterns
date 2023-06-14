#include "stdafx.h"
#include "per_game.h"
#include "player_ai_component.h"
#include "player_input_component.h"
#include "player_physics_component.h"
#include "player_graphics_component.h"

PERGame::PERGame()
{
	m_controller = new PERController();
	m_renderer = new PERRenderer();
	m_world = new PERWorld();
	m_player = new PERObject(
		new PlayerInputComponent(),
		new PlayerAiComponent(),
		new PlayerPhysicsComponent(),
		new PlayerGraphicsComponent()
	);
}

PERGame::~PERGame()
{
	delete m_controller;
	delete m_renderer;
	delete m_world;
}

void PERGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_UP, isDown);
	if (wParam == VK_DOWN) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN, isDown);
	if (wParam == VK_LEFT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT, isDown);
	if (wParam == VK_RIGHT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT, isDown);
	if (wParam == VK_SPACE) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_SPACE, isDown);
}

void PERGame::Update(int time)
{
	m_updateLag += time;

	// PER_MILLISEC_PER_UPDATE ��ŭ�� ������Ʈ
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// ������ �ð���ŭ ���� ���� ������Ʈ
		UpdateControllerAndWorld(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// �ִ� ������Ʈ ���� Ƚ���� �Ѿ ���� ��츦 ����� ������Ʈ�� �ɸ��� �ð����� ����
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
}

void PERGame::Render(HWND hWnd)
{
	HDC hDC, memDC;
	HBITMAP hBitmap, oldBitmap;
	RECT rect;

	hDC = GetDC(hWnd);

	// ���� ���۸� ���� ����
	GetClientRect(hWnd, &rect);
	memDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	// ��ü ��� �ʱ�ȭ
	Rectangle(memDC, -1, -1, rect.right + 1, rect.bottom + 1);

	// ���� ���� ������
	RenderWorld(hWnd, memDC);

	// ���� ��� ���۷� �̵�
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

	// ���� ���۸����� ������ ����
	DeleteObject(SelectObject(memDC, oldBitmap));
	DeleteDC(memDC);
	DeleteDC(hDC);
	DeleteObject(hBitmap);
	DeleteObject(oldBitmap);
}

PERController& PERGame::GetController()
{
	return *m_controller;
}

PERRenderer& PERGame::GetRenderer()
{
	return *m_renderer;
}

void PERGame::UpdateControllerAndWorld(double dTime)
{
	m_controller->Update(dTime);
	m_player->Update(*m_controller, *m_world, dTime);
}

void PERGame::RenderWorld(HWND hWnd, HDC memDC)
{
	// �������� �ִ� �޸� DC�� ���� �޸� DC�� ����
	m_renderer->MatchWindowSizeAndCurrentMemoryDC(hWnd, memDC);

	// ���� ������
	m_renderer->RenderWorld(*m_world);
	m_player->Render(*m_renderer);
}
