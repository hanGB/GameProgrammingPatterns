#pragma once
#include "sl_renderer.h"
#include "sl_controller.h"
#include "sl_audio.h"

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

	void EnableAudio();
	void DisableAudio();
	void EnableAudioLogging();
	void DisableAudioLogging();

	static const int c_HERO_SPEED = 2;

	SlVector3<double> m_heroPosition;

	bool m_isEnabledAudio;
	bool m_isEnabledAudioLogging;

	SlRenderer m_renderer;
	SlController m_controller;
	SlAudio* m_audio;
	SlAudio* m_loggedAudio;
};