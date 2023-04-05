#pragma once
#include "stdafx.h"
#include "eq_game.h"
#include "eq_controller.h"
#include "eq_sound_player.h"

EqGame::EqGame()
{
	InitGame();
}

EqGame::~EqGame()
{
	CleanupGame();
}

void EqGame::HandleKeyboardInput(WPARAM wParam, bool isDown, double elapsedTimeInSec)
{
	EqController::GetInstance().UpdateInputData(wParam, isDown, elapsedTimeInSec);
}

void EqGame::Update(double elapsedTimeInSec)
{
	EqSoundPlayer::GetInstance().Update(elapsedTimeInSec);
	m_userInterface->Update(elapsedTimeInSec);
}

void EqGame::Render(HWND hWnd, HDC& memDC)
{
	m_renderer->SetWindowSizeAndNowFrameMemoryDC(hWnd, memDC);
	m_userInterface->Render(*m_renderer);
}

void EqGame::InitGame()
{
	m_renderer = new EqRenderer();
	m_userInterface = new EqUserInterface();
}

void EqGame::CleanupGame()
{
	delete m_renderer;
	delete m_userInterface;
}
