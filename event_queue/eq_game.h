#pragma once
#include "eq_renderer.h"

class EqGame {
public:
	EqGame(HWND hWnd);
	~EqGame();

	void HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	void Update(double g_elapsedTimeInSec);
	void Render(HDC& memDC);

private:
	void InitGame(HWND hWnd);
	void CleanupGame();

	EqRenderer* m_renderer;

	int m_selectedMenu = 0;
};