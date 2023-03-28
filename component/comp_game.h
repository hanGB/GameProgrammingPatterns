#pragma once
#include "comp_renderer.h"
#include "comp_world.h"
#include "comp_object.h"

class CompGame {
public:
	CompGame(HWND hWnd);
	~CompGame();

	void HandleKeyboardInput(bool isDown, WPARAM wParam);

	void Update(double elapsedTimeInSec);
	void Render(HDC& memDC);

private:
	void InitGame(HWND hWnd);
	void CleanupGame();

	CompRenderer* m_renderer;
	CompWorld* m_world;
	CompObject* m_bjorn;

};