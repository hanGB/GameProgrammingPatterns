#pragma once
#include "per_controller.h"

class PERGame {
public:
	PERGame();
	~PERGame();

	void HandleInput(WPARAM wParam, bool isDown);
	void Update(int deltaTime);
	void Render(HWND m_hWnd);

	PERController& GetController();

private:
	void UpdateControllerAndWorld(double dTime);
	void RenderWorld(HDC memDC);

	// 컨트롤러
	PERController* m_controller;

	CoordinateData m_coordinateData;
	int m_updateLag = 0;

	double m_x = 0.0, m_y = 0.0;
	double m_speed = 3.0;
};