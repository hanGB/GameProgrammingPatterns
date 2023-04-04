#pragma once
#include "stdafx.h"
#include "eq_game.h"
#include "eq_controller.h"

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
	EqController::GetInstance()->UpdateInputData(wParam, isDown, elapsedTimeInSec);
}

void EqGame::Update(double g_elapsedTimeInSec)
{
	EqController* controller = EqController::GetInstance();

	if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_W)
		|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_W)) 
	{
		m_selectedMenu--;
	}
	if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_S)
		|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_S))
	{
		m_selectedMenu++;
	}

	if (m_selectedMenu < 0) m_selectedMenu = 0;
	else if (m_selectedMenu > 4) m_selectedMenu = 4;
}

void EqGame::Render(HWND hWnd, HDC& memDC)
{
	m_renderer->SetWindowSizeAndNowFrameMemoryDC(hWnd, memDC);
	
	EqVector3<double> pos = { -3.5, 2.0 };
	EqVector2<double> size = { 2.5, 0.5 };
	EqColor color;
	EqColor selectedColor = {127, 0, 0};
	EqColor nonSelectedColor = { 127, 127, 127};

	for (int i = 0; i < 5; ++i) {
		color = nonSelectedColor;
		if (m_selectedMenu == i) color = selectedColor;
		m_renderer->RenderShape(EqShapeType::EQ_SHAPE_TYPE_RECTANGLE, pos, size, color);

		EqVector2<double> textPos = { pos.x - (size.x / 2.0), pos.y + (size.y / 2.0) };
		m_renderer->RenderFont(L"테스트 출력문", sizeof(L"테스트 출력문") / sizeof(wchar_t), 0.3, textPos, { 255, 255, 255 });
		pos.y -= 1.0;
	}
}

void EqGame::InitGame()
{
	m_renderer = new EqRenderer();
}

void EqGame::CleanupGame()
{
	delete m_renderer;
}
