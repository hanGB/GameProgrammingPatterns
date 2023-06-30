#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_world.h"
#include "object_pool.h"
#include "per_object.h"

class PERGame {
public:
	PERGame(HWND hWnd);
	~PERGame();

	void HandleInput(WPARAM wParam, bool isDown);
	void Update(int deltaTime);
	void UIUpdate(int deltaTime);
	void Render(HWND hWnd);
	void UIRender(HWND hWnd);

	PERController& GetController();
	PERRenderer& GetRenderer();

private:
	const double c_FPS_UPDATE_GAP = 0.5;

	int m_updateLag = 0;
	std::atomic<double> m_frameGap;
	std::atomic<bool> m_isUpdateEnd = false;
	std::atomic<bool> m_isRenderEnd = false;
	// ui
	std::atomic<bool> m_isUpdateUIEnd = false;
	std::atomic<bool> m_isRenderUIEnd = false;

	PERController*	m_controller;
	PERRenderer*	m_renderer;
	ObjectPool*	m_objectPool;

	PERObject*	m_player;
	PERWorld*	m_world;

	// 프레임 측정 관련
	double m_fpsUpdateTime = 0.0;
	wchar_t m_fpsText[10];
	std::atomic<int> m_fps;
};