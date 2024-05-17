#include "stdafx.h"
#include "title_hud.h"
#include "player_state.h"

TitleHud::TitleHud()
{
	// 게임 타이틀
	TextViewer* title = dynamic_cast<TextViewer*>(m_uiElementPool->Create(PERUiElementType::TEXT_VIEWER));
	if (title) title->Init(PERVec2(-0.8, 0.8), 0.3, PERColor(100, 0, 0), "TEXT_TITLE");
	else PERLog::Logger().ErrorWithFormat("title hud의 타이틀이 생성되지 않았습니다.");

	std::string text[3] = { "TEXT_START", "TEXT_CHANGE_WINDOW_SIZE", "TEXT_EXIT" };

	for (int i = 0; i < m_numMenu; ++i)
	{
		m_menu[i] = dynamic_cast<SelectBox*>(m_uiElementPool->Create(PERUiElementType::SELECT_BOX));
		if (m_menu[i]) m_menu[i]->Init(PERVec2(-0.8, -0.2 - 0.2 * i), PERVec2(0.5, 0.15), 
			true, PERColor(200, 200, 200), true, PERColor(150, 150, 150),
			true, 1, PERColor(0, 0, 0), false, 1, PERColor(0, 0, 0),
			true, text[i], 0.1, PERVec2(-0.79, -0.2 - 0.2 * i), PERColor(0, 0, 0), PERColor(50, 50, 50));
		else PERLog::Logger().ErrorWithFormat("%s 메뉴가 생성되지 않았습니다.", text[i]);
	}
}

TitleHud::~TitleHud()
{
}

void TitleHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::SET_SELECTED_MENU:
		m_selectedMenu = (int)data.x;
		ChangeSelectedMenu();
		break;
	}
}

void TitleHud::MatchWithPlayerState(PlayerState& state)
{
	ChangeSelectedMenu();
}

void TitleHud::ChangeSelectedMenu()
{
	for (int i = 0; i < m_numMenu; ++i)
	{
		bool on = false;
		if (i == m_selectedMenu) on = true;
		m_menu[i]->MatchWithData(PERVec2(-0.8, -0.2 - i * 0.2), PERVec2(-0.79, -0.2 - i * 0.2), on);
	}
}
