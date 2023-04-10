#pragma once
#include "stdafx.h"
#include "sl_game.h"
#include "sl_locator.h"
#include "sl_controller.h"
#include "sl_renderer.h"
#include "sl_windows_audio.h"
#include "sl_logged_audio.h"

SlGame::SlGame()
{
	InitGame();
}

SlGame::~SlGame()
{
	CleanupGame();
}

void SlGame::HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec)
{
	m_controller.UpdateInputData(wParam, isDown, elapsedTimeInSec);
}

void SlGame::Update(double elapsedTimeInSec)
{
	m_heroPosition.x += c_HERO_SPEED * m_controller.GetXAxisDirection() * elapsedTimeInSec;
	m_heroPosition.y += c_HERO_SPEED * m_controller.GetYAxisDirection() * elapsedTimeInSec;

	if (m_controller.IsDownOnceOrMoreThanTIme(SlInputKeyValue::INPUT_SPACE)) {
		SlLocator::GetAudio().PlaySound(SlSoundId::SOUND_BANG);
	}
	if (m_controller.IsDownOnce(SlInputKeyValue::INPUT_C)) {
		m_isEnabledAudio = (m_isEnabledAudio + 1) % 2;

		if (m_isEnabledAudio) EnableAudio();
		else DisableAudio();
	}
	if (m_controller.IsDownOnce(SlInputKeyValue::INPUT_L)) {
		if (m_isEnabledAudio) {
			m_isEnabledAudioLogging = (m_isEnabledAudioLogging + 1) % 2;

			if (m_isEnabledAudioLogging) EnableAudioLogging();
			else DisableAudioLogging();
		}
	}
}

void SlGame::Render(HWND hWnd, HDC& memDC)
{
	m_renderer.SetWindowSizeAndNowFrameMemoryDC(hWnd, memDC);
	m_renderer.RenderShape(SlShapeType::SHAPE_TYPE_ELLIPSE, m_heroPosition, { 0.5, 0.5 }, { 255, 0, 0 });

}

void SlGame::InitGame()
{
	m_heroPosition = { 0.0, 0.0, 0.0 };

	m_audio = new SlWindowsAudio();
	SlLocator::ProvideAudio(m_audio);
	m_loggedAudio = nullptr;

	m_isEnabledAudio = true;
	m_isEnabledAudioLogging = false;
}

void SlGame::CleanupGame()
{
	delete m_audio;
	if (m_loggedAudio) delete m_loggedAudio;
}

void SlGame::EnableAudio()
{
	std::cout << "오디오를 활성화 합니다\n";
	if (m_isEnabledAudioLogging) SlLocator::ProvideAudio(m_loggedAudio);
	else SlLocator::ProvideAudio(m_audio);
}

void SlGame::DisableAudio()
{
	std::cout << "오디오를 비활성화 합니다\n";
	SlLocator::ProvideAudio(nullptr);
}

void SlGame::EnableAudioLogging()
{
	std::cout << "오디오 로그를 활성화 합니다\n";
	m_loggedAudio = new SlLoggedAudio(SlLocator::GetAudio());
	SlLocator::ProvideAudio(m_loggedAudio);
}

void SlGame::DisableAudioLogging()
{
	std::cout << "오디오 로그를 비활성화 합니다\n";
	SlLocator::ProvideAudio(m_audio);
	delete m_loggedAudio;
	m_loggedAudio = nullptr;
}
