#pragma once
#include "per_controller.h"
#include "per_renderer.h"
#include "per_audio.h"
#include "per_world.h"
#include "game_mode.h"
#include "object_storage.h"
#include "per_object.h"
#include "event_reciver.h"
#include "per_database.h"

class PERGame : public EventReciver {
public:
	PERGame(HWND hWnd);
	~PERGame();

	virtual void Recive(PEREvent event, PERVec3 data);

	void HandleInput(WPARAM wParam, bool isDown);

	void Update(int deltaTime);
	void UIUpdate(int deltaTime);
	void Render(HWND hWnd);
	void UIRender(HWND hWnd);

	void AudioUpdate();

	// 윈도우 hwnd 재설정
	void MatchWindowHWND(HWND hWnd);

private:
	// 게임 월드, 모드 변경
	void Run(PERWorld* world, GameMode* gameMode);
	void ChangeWorld(PERWorld* world, GameMode* gameMode);
	void PushWorld(PERWorld* world, GameMode* gameMode);
	void PopWorld();
	void Quit();

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
	PERAudio*		m_audio;
	ObjectStorage*	m_objectStorage;
	PERDatabase*	m_database;

	PERWorld*	m_currentWorld;
	GameMode*   m_currentGameMode;

	// 프레임 측정 관련
	double m_fpsUpdateTime = 0.0;
	wchar_t m_fpsText[10];
	std::atomic<int> m_fps;
};