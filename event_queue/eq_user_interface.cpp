#include "stdafx.h"
#include "eq_user_interface.h"
#include "eq_controller.h"

EqUserInterface::EqUserInterface()
{
}

EqUserInterface::~EqUserInterface()
{
}

void EqUserInterface::Update(double elapsedTimeInSec)
{
	UpdateMenuWithControl();
	UseSelectedAct();
}

void EqUserInterface::Render(EqRenderer& renderer)
{
	RenderTurns(renderer);
	
	switch (m_menuId) {
	case EqMenuId::EQ_MENU_ID_MAIN:
		RenderMainMenu(renderer);
		break;
	case EqMenuId::EQ_MENU_ID_SKILL:
		RenderSubMenu(renderer);
		break;
	case EqMenuId::EQ_MENU_ID_ITEM:
		RenderSubMenu(renderer);
		break;
	}
}

void EqUserInterface::UpdateMenuWithControl()
{
	EqController* controller = EqController::GetInstance();

	switch (m_menuId) {
	case EqMenuId::EQ_MENU_ID_MAIN: {
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_W)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_W))
		{
			m_actId = EqActId::EQ_ACT_ID_MAIN;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_S)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_S))
		{
			m_actId = (EqActId)((int)m_actId + 1);
		}
		if (m_actId == EqActId::EQ_LAST_ACT) m_actId = EqActId::EQ_ACT_ID_CHANGE;

		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_E))
		{
			m_menuId = EqMenuId::EQ_MENU_ID_SKILL;
			return;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_F))
		{
			m_menuId = EqMenuId::EQ_MENU_ID_ITEM;
			return;
		}

		if (m_actId != EqActId::EQ_ACT_ID_MAIN) {
			if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SPACE))
			{
				m_menuId = EqMenuId::EQ_MENU_ID_ACT;
				return;
			}
			if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SHIFT))
			{
				m_actId = EqActId::EQ_ACT_ID_MAIN;
				return;
			}
		}

		break;
	}
	case EqMenuId::EQ_MENU_ID_SKILL: {
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_W)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_W))
		{
			m_selectedMenuObjectInSkill--;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_S)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_S))
		{
			m_selectedMenuObjectInSkill++;
		}
		if (m_selectedMenuObjectInSkill < 0) m_selectedMenuObjectInSkill = 0;
		else if (m_selectedMenuObjectInSkill > 3) m_selectedMenuObjectInSkill = 3;

		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SPACE))
		{
			std::cout << m_selectedMenuObjectInSkill << "번 스킬을 사용했습니다.\n";
			return;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SHIFT))
		{
			m_menuId = EqMenuId::EQ_MENU_ID_MAIN;
			return;
		}
		break;

	}
	case EqMenuId::EQ_MENU_ID_ITEM: {
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_W)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_W))
		{
			m_selectedMenuObjectInItem--;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_S)
			|| controller->IsDownMoreThanTime(EqInputKeyValue::EQ_INPUT_S))
		{
			m_selectedMenuObjectInItem++;
		}
		if (m_selectedMenuObjectInItem < 0) m_selectedMenuObjectInItem = 0;
		else if (m_selectedMenuObjectInItem > 3) m_selectedMenuObjectInItem = 3;

		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SPACE))
		{
			std::cout << m_selectedMenuObjectInItem << "번 아이템을 사용했습니다.\n";
			return;
		}
		if (controller->IsDownOnce(EqInputKeyValue::EQ_INPUT_SHIFT))
		{
			m_menuId = EqMenuId::EQ_MENU_ID_MAIN;
			return;
		}
		break;
	}
	}
}

void EqUserInterface::UseSelectedAct()
{
	if (m_menuId == EqMenuId::EQ_MENU_ID_ACT) {
		bool isEnd = false;

		switch (m_actId) {
		case EqActId::EQ_ACT_ID_CHANGE:
			std::cout << "무언가를 변경합니다\n";
			isEnd = true;
			break;
		case EqActId::EQ_ACT_ID_WAIT:
			std::cout << "아무것도 안합니다\n";
			isEnd = true;
			break;
		case EqActId::EQ_ACT_ID_RUNAWAY:
			std::cout << "전투에서 도망갑니다\n";
			isEnd = true;
			break;
		}

		if (isEnd) 
		{
			m_menuId = EqMenuId::EQ_MENU_ID_MAIN;
			m_actId = EqActId::EQ_ACT_ID_MAIN;
		}
	}
}

void EqUserInterface::RenderMainMenu(EqRenderer& renderer)
{
	EqVector3<double> pos = { -4.3, -1.65 };
	EqVector2<double> size = { 0.4, 0.4 };
	EqVector2<double> textPos;

	for (int i = 0; i < 3; ++i) {
		renderer.RenderShape(EqShapeType::EQ_SHAPE_TYPE_RECTANGLE, pos, size, { 200, 200, 200 });

		textPos = { pos.x + 0.08 - (size.x / 2.0), pos.y + 0.08 + (size.y / 2.0) };
		switch (i) {
		case 0:
			renderer.RenderFont(L"E", sizeof(L"E") / sizeof(wchar_t), 0.5, textPos, { 0, 0, 0 });
			break;
		case 1:
			renderer.RenderFont(L"F", sizeof(L"E") / sizeof(wchar_t), 0.5, textPos, { 0, 0, 0 });
			break;
		case 2:
			renderer.RenderFont(L"S", sizeof(L"E") / sizeof(wchar_t), 0.5, textPos, { 0, 0, 0 });
			break;
		}
		pos.y -= 0.5;
	}

	pos = { -3.1, -1.65 };
	size = { 1.8, 0.4 };
	for (int i = 0; i < 3; ++i) {
		renderer.RenderShape(EqShapeType::EQ_SHAPE_TYPE_RECTANGLE, pos, size, { 200, 200, 200 });

		textPos = { pos.x + 0.5 - (size.x / 2.0), pos.y + 0.03 + (size.y / 2.0) };
		switch (i) {
		case 0:
			renderer.RenderFont(L"skill", sizeof(L"skill") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
			break;
		case 1:
			renderer.RenderFont(L"item", sizeof(L"item") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
			break;
		case 2: 
		{
			switch (m_actId) {
			case EqActId::EQ_ACT_ID_MAIN:
				renderer.RenderFont(L"act", sizeof(L"act") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
				break;
			case EqActId::EQ_ACT_ID_CHANGE:
				textPos = { pos.x + 0.4 - (size.x / 2.0), pos.y + 0.03 + (size.y / 2.0) };
				renderer.RenderFont(L"change", sizeof(L"change") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
				break;
			case EqActId::EQ_ACT_ID_WAIT:
				renderer.RenderFont(L"wait", sizeof(L"wait") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
				break;
			case EqActId::EQ_ACT_ID_RUNAWAY:
				textPos = { pos.x + 0.3 - (size.x / 2.0), pos.y + 0.03 + (size.y / 2.0) };
				renderer.RenderFont(L"runaway", sizeof(L"runaway") / sizeof(wchar_t), 0.4, textPos, { 0, 0, 0 });
				break;
			}		
			break;
		}
		}
		pos.y -= 0.5;
	}
}

void EqUserInterface::RenderSubMenu(EqRenderer& renderer)
{
	EqVector3<double> pos = { -4.5, -1.15 };
	EqVector2<double> size = { 1.8, 0.4 };
	EqVector2<double> textPos = { pos.x - 0.1 - (size.x / 2.0), pos.y + 0.03 + (size.y / 2.0) };

	//renderer.RenderShape(EqShapeType::EQ_SHAPE_TYPE_RECTANGLE, pos, size, { 200, 200, 200 });

	int selectedObject = 0;
	switch (m_menuId) {
	case EqMenuId::EQ_MENU_ID_SKILL:
		selectedObject = m_selectedMenuObjectInSkill;
		renderer.RenderFont(L"skill", sizeof(L"skill") / sizeof(wchar_t), 0.6, textPos, { 0, 0, 0 });
		textPos = { textPos.x + 0.03, textPos.y + 0.03 };
		renderer.RenderFont(L"skill", sizeof(L"skill") / sizeof(wchar_t), 0.6, textPos, { 200, 200, 200 });
		break;
	case EqMenuId::EQ_MENU_ID_ITEM:
		selectedObject = m_selectedMenuObjectInItem;
		renderer.RenderFont(L"item", sizeof(L"item") / sizeof(wchar_t), 0.6, textPos, { 0, 0, 0 });
		textPos = { textPos.x + 0.03, textPos.y + 0.03 };
		renderer.RenderFont(L"item", sizeof(L"item") / sizeof(wchar_t), 0.6, textPos, { 200, 200, 200 });
		break;
	}
	
	pos = { -3.2, -1.6 };
	size = { 2.5, 0.4 };

	bool isSelected = false;
	for (int i = 0; i < 4; ++i) {
		if (i == selectedObject) isSelected = true;

		if (isSelected) pos.x += 0.5;
		renderer.RenderShape(EqShapeType::EQ_SHAPE_TYPE_RECTANGLE, pos, size, { 200, 200, 200 });
		textPos = { pos.x + 0.1 - (size.x / 2.0), pos.y - 0.04 + (size.y / 2.0) };

		wchar_t text[20] = L" ";
		if (m_menuId == EqMenuId::EQ_MENU_ID_SKILL) wsprintf(text, L"skill %d data", i);
		else if (m_menuId == EqMenuId::EQ_MENU_ID_ITEM) wsprintf(text, L"item %d data", i);

		int textBufferSize = 0;
		for (int n = 0; n < 20; ++n) {
			if (text[n] == '\0') {
				textBufferSize = n;
				break;
			}
		}
		renderer.RenderFont(text, textBufferSize, 0.3, textPos, { 0, 0, 0 });

		wchar_t amoutText[10];
		if (m_menuId == EqMenuId::EQ_MENU_ID_SKILL) wsprintf(text, L"|  %d", (i + 1) * 10);
		else if (m_menuId == EqMenuId::EQ_MENU_ID_ITEM) wsprintf(text, L"|   %d", i + 1);
		textPos.x += 1.8;
		for (int n = 0; n < 20; ++n) {
			if (text[n] == '\0') {
				textBufferSize = n;
				break;
			}
		}
		renderer.RenderFont(text, textBufferSize, 0.3, textPos, { 0, 0, 0 });

		if (isSelected) {
			pos.x -= 0.5;
			isSelected = false;
		}
		pos.y -= 0.4;
	}
}

void EqUserInterface::RenderTurns(EqRenderer& renderer)
{
	wchar_t turnText[2];
	wsprintf(turnText, L"%d", m_turns);

	renderer.RenderFont(turnText, sizeof(turnText) / sizeof(wchar_t), 2, { -5.6, -1.1 }, { 0, 0, 0 });
	renderer.RenderFont(turnText, sizeof(turnText) / sizeof(wchar_t), 2, { -5.5, -1.0 }, { 200, 200, 200 });
}
