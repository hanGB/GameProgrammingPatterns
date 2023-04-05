#pragma once
#include "eq_renderer.h"
#include "eq_user_interface.h"

class EqGame {
public:
	EqGame();
	~EqGame();

	void HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	void Update(double elapsedTimeInSec);
	void Render(HWND hWnd, HDC& memDC);

private:
	void InitGame();
	void CleanupGame();

	EqRenderer* m_renderer;
	EqUserInterface* m_userInterface;
};