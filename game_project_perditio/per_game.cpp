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
	double dTime = time / 1'000'000.0;

	m_controller->Update(dTime);
	m_world->InputUpdate(*m_controller, dTime);

	// ������ �ð���ŭ ������Ʈ�� �ʿ��� �׸� ������Ʈ
	m_updateLag += time;
	// PER_MILLISEC_PER_UPDATE ��ŭ�� ������Ʈ
	for (int i = 0; i < PER_MAXIMUM_UPDATE_LOOP_COUNT && m_updateLag >= PER_MICROSEC_PER_UPDATE; ++i) {
		// ������ �ð���ŭ ���� ���� ������Ʈ
		m_world->AiUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_world->PhysicsUpdate(PER_MICROSEC_PER_UPDATE / 1'000'000.0);
		m_updateLag -= PER_MICROSEC_PER_UPDATE;
	}
	// �ִ� ������Ʈ ���� Ƚ���� �Ѿ ���� ��츦 ����� ������Ʈ�� �ɸ��� �ð����� ����
	m_updateLag %= PER_MICROSEC_PER_UPDATE;

	m_world->GraphicsUpdate(dTime);
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

	// �������� �ִ� �޸� DC�� ���� �޸� DC�� ����
	m_renderer->MatchWindowSizeAndCurrentMemoryDC(hWnd, memDC);
	// ���� ���� ������
	m_world->Render(*m_renderer);

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

