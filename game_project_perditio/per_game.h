#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_world.h"
#include "game_mode.h"
#include "object_pool.h"
#include "per_object.h"

class PERGame {
public:
	static PERGame& Instance();
	void InitRenderer(HWND hWnd);
	~PERGame();

	void HandleInput(WPARAM wParam, bool isDown);
	void Update(int deltaTime);
	void UIUpdate(int deltaTime);
	void Render(HWND hWnd);
	void UIRender(HWND hWnd);

	// ������ hwnd �缳��
	void MatchWindowHWND(HWND hWnd);

	// ������Ʈ ����, ����
	PERObject* CreateObject(PERObjectType type);
	void RemoveObject(PERObjectType type, PERObject* object);

	// ���� ����, ��� ����
	void Run(PERWorld* world, GameMode* gameMode);
	void ChangeWorld(PERWorld* world, GameMode* gameMode);
	void PushWorld(PERWorld* world, GameMode* gameMode);
	void PopWorld();
	void Quit();

private:
	PERGame();

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
	PERWorld*	m_currentWorld;
	GameMode*   m_currentGameMode;

	// ������ ���� ����
	double m_fpsUpdateTime = 0.0;
	wchar_t m_fpsText[10];
	std::atomic<int> m_fps;
};