#include "stdafx.h"
#include "per_game.h"

PERGame::PERGame()
{
	m_coordinateData.halfWidth = PER_WINDOW_WIDTH / 2;
	m_coordinateData.halfHeight = PER_WINDOW_HEIGHT / 2;

	m_controller = new PERController();
}

PERGame::~PERGame()
{
	delete m_controller;
}

void PERGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_UP, isDown);
	if (wParam == VK_DOWN) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN, isDown);
	if (wParam == VK_LEFT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT, isDown);
	if (wParam == VK_RIGHT) m_controller->SetKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT, isDown);
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
	RenderWorld(memDC);

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

void PERGame::UpdateControllerAndWorld(double dTime)
{
	m_controller->Update(dTime);

	if (m_controller->IsKeyboardPressed(PERKeyboardValue::KEYBOARD_UP)) m_y += m_speed * dTime;
	if (m_controller->IsKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN)) m_y -= m_speed * dTime;
	if (m_controller->IsKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT)) m_x -= m_speed * dTime;
	if (m_controller->IsKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT)) m_x += m_speed * dTime;
}

void PERGame::RenderWorld(HDC memDC)
{
	double winX = m_x, winY = m_y;

	m_coordinateData.ConvertCoordinateOpenGLToWindows(&winX, &winY);
	Rectangle(memDC, 
		(int)((winX - 0.5)* PER_PIXEL_PER_METER), 
		(int)((winY - 0.5)* PER_PIXEL_PER_METER),
		(int)((winX + 0.5)* PER_PIXEL_PER_METER), 
		(int)((winY + 0.5) * PER_PIXEL_PER_METER));
}
