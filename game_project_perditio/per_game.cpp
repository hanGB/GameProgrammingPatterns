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

	// PER_MILLISEC_PER_UPDATE 만큼씩 업데이트
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// 정해진 시간만큼 게임 월드 업데이트
		UpdateWorld(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// 최대 업데이트 루프 횟수를 넘어서 끝날 경우를 대비해 업데이트에 걸리는 시간으로 나눔
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
}

void PerGame::Render(HWND hWnd)
{
	HDC hDC, memDC;
	HBITMAP hBitmap, oldBitmap;
	RECT rect;

	hDC = GetDC(hWnd);

	// 더블 버퍼링 관련 설정
	GetClientRect(hWnd, &rect);
	memDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	// 전체 흰색 초기화
	Rectangle(memDC, -1, -1, rect.right + 1, rect.bottom + 1);

	// 게임 월드 렌더링
	RenderWorld(memDC);

	// 실제 출력 버퍼로 이동
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

	// 더블 버퍼링관련 데이터 삭제
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
