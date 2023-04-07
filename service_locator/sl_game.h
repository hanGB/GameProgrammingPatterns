#pragma once
#include "sl_renderer.h"
#include "sl_controller.h"

class SlGame {
public:
	SlGame();
	~SlGame();

	void HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec);

	void Update(double elapsedTimeInSec);
	void Render(HWND hWnd, HDC& memDC);

private:
	void InitGame();
	void CleanupGame();

	static const int c_HERO_SPEED = 2;

	SlVector3<double> m_heroPosition;

	SlRenderer m_renderer;
	SlController m_controller;
};