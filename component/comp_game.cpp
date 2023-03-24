#pragma once
#include "stdafx.h"
#include "comp_game.h"
#include "comp_controller.h"

CompGame::CompGame(HWND hWnd)
{
	InitGame(hWnd);
}

CompGame::~CompGame()
{
	CleanupGame(); 
}

void CompGame::HandleKeyboardInput(bool isDown, WPARAM wParam)
{
	CompInputChunk inputs = CompController::GetInstance()->GetInputs();

	if (wParam == 'w' || wParam == 'W') inputs.Input_W = isDown;
	if (wParam == 'a' || wParam == 'A') inputs.Input_A = isDown;
	if (wParam == 's' || wParam == 'S') inputs.Input_S = isDown;
	if (wParam == 'd' || wParam == 'D') inputs.Input_D = isDown;

	if (wParam == VK_SPACE)	inputs.Input_Space = isDown;

	CompController::GetInstance()->SetInputs(inputs);
}

void CompGame::Update(double elapsedTimeInSec)
{
	m_bjorn->Update(*m_world, elapsedTimeInSec);
}

void CompGame::Render(HDC& memDC)
{
	m_renderer->SetNowFrameMemoryDC(memDC);
	m_world->RenderWorld(*m_renderer);
	m_bjorn->Render(*m_renderer);
}

void CompGame::InitGame(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	m_renderer = new CompRenderer((double)rect.right / 2.0, (double)rect.bottom / 2.0);
	m_world = new CompWorld();
	m_bjorn = new Bjorn();
	
}

void CompGame::CleanupGame()
{
	delete m_renderer;
	delete m_world;
	delete m_bjorn;
}
