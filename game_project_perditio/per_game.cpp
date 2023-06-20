#include "stdafx.h"
#include "per_game.h"

PERGame::PERGame()
{
	m_controller = new PERController();
	m_renderer = new PERRenderer();
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
}

void PERGame::Update(int time)
{
	m_updateLag += time;

	// PER_MILLISEC_PER_UPDATE 만큼씩 업데이트
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// 정해진 시간만큼 게임 월드 업데이트
		UpdateControllerAndWorld(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// 최대 업데이트 루프 횟수를 넘어서 끝날 경우를 대비해 업데이트에 걸리는 시간으로 나눔
	m_updateLag %= PER_MICROSEC_PER_UPDATE;
}

void PERGame::Render(HWND hWnd)
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
	MatchFrameAndRenderWorld(hWnd, memDC);

	// 실제 출력 버퍼로 이동
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

	// 더블 버퍼링관련 데이터 삭제
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
	m_world->Update(*m_controller, dTime);
}

void PERGame::MatchFrameAndRenderWorld(HWND hWnd, HDC memDC)
{
	// 렌더러에 있는 메모리 DC를 현재 메모리 DC로 맞춤
	m_renderer->MatchWindowSizeAndCurrentMemoryDC(hWnd, memDC);
	m_world->Render(*m_renderer);
}
