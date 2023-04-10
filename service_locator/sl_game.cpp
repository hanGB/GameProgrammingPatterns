#pragma once
#include "stdafx.h"
#include "sl_game.h"
#include "sl_locator.h"
#include "sl_controller.h"
#include "sl_renderer.h"

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
		SlLocator::GetAudio()->PlaySound(SlSoundId::SOUND_BANG);
	}
	if (m_controller.IsDownOnce(SlInputKeyValue::INPUT_C)) {
		m_isUseAudio = (m_isUseAudio + 1) % 2;

		if (m_isUseAudio) {
			SlLocator::ProvideAudio(m_windowsAudio);
			std::cout << "오디오를 사용합니다\n";
		}
		else {
			SlLocator::ProvideAudio(nullptr);
			std::cout << "오디오를 사용하지 않습니다\n";
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

	m_windowsAudio = new SlWindowsAudio();
	SlLocator::ProvideAudio(m_windowsAudio);

	m_isUseAudio = true;
}

void SlGame::CleanupGame()
{
	delete m_windowsAudio;
}
