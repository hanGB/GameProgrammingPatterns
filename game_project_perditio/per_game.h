#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_world.h"
#include "object_pool.h"
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
	void MatchFrameAndRenderWorld(HWND hWnd, HDC memDC);

	int m_updateLag = 0;

	PERController*	m_controller;
	PERRenderer*	m_renderer;
	ObjectPool*	m_objectPool;

	PERObject*	m_player;
	PERWorld*	m_world;
};