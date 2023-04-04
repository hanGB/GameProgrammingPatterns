#pragma once
#include "eq_renderer.h"

class EqGame {
public:
	EqGame();
	~EqGame();

	void HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	void Update(double g_elapsedTimeInSec);
	void Render(HWND hWnd, HDC& memDC);

private:
	void InitGame();
	void CleanupGame();

	EqRenderer* m_renderer;

	int m_selectedMenu = 0;
};