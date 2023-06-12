#include "stdafx.h"
#include "per_game.h"

PerGame::PerGame()
{
	m_coordinateData.halfWidth = PER_WINDOW_WIDTH / 2;
	m_coordinateData.halfHeight = PER_WINDOW_HEIGHT / 2;
}

PerGame::~PerGame()
{
}

void PerGame::HandleInput(WPARAM wParam, bool isDown)
{
	if (wParam == VK_UP) m_flag = isDown;
}

void PerGame::Update(int time)
{
	m_updateLag += time;

	// PER_MILLISEC_PER_UPDATE ��ŭ�� ������Ʈ
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// ������ �ð���ŭ ���� ���� ������Ʈ
		UpdateWorld(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// �ִ� ������Ʈ ���� Ƚ���� �Ѿ ���� ��츦 ����� ������Ʈ�� �ɸ��� �ð����� ����
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
}

void PerGame::Render(HWND hWnd)
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

void PerGame::UpdateWorld(double deltaTime)
{
	
}

void PerGame::RenderWorld(HDC memDC)
{
	if (m_flag) Rectangle(memDC, 100, 100, 200, 200);
}
