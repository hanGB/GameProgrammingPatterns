#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_world.h"
#include "per_object.h"

class PERGame {
public:
	PERGame();
	~PERGame();

	void HandleInput(WPARAM wParam, bool isDown);
	void Update(int deltaTime);
	void Render(HWND m_hWnd);

	PERController& GetController();
	PERRenderer& GetRenderer();

private:
	void UpdateControllerAndWorld(double dTime);
	void RenderWorld(HWND hWnd, HDC memDC);

	PERController*	m_controller;
	PERRenderer*	m_renderer;

	int m_updateLag = 0;

	PERWorld*		m_world;
	PERObject*		m_player;
};