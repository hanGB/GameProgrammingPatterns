#include "stdafx.h"
#include "respawn_hud.h"

RespawnHud::RespawnHud()
{
	std::string text[2] = { "TEXT_RESPAWN", "TEXT_BACK_TO_TITLE" };

	for (int i = 0; i < m_numMenu; ++i)
	{
		m_menu[i] = dynamic_cast<SelectBox*>(m_uiElementPool->Create(PERUiElementType::SELECT_BOX));
		if (m_menu[i]) m_menu[i]->Init(PERVec2(0.2, -0.4 - 0.2 * i), PERVec2(0.5, 0.15),
			true, PERColor(200, 200, 200), false, PERColor(150, 150, 150),
			true, 1, PERColor(0, 0, 0), false, 1, PERColor(0, 0, 0),
			true, text[i], 0.1, PERVec2(0.21, -0.4 - 0.2 * i), PERColor(0, 0, 0), PERColor(50, 50, 50));
		else PERLog::Logger().ErrorWithFormat("%s 메뉴가 생성되지 않았습니다.", text[i]);
	}
}

RespawnHud::~RespawnHud()
{
}

void RespawnHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::SET_SELECTED_MENU:
		m_selectedMenu = (int)data.x;
		ChangeSelectedMenu();
		break;
	}
}

void RespawnHud::MatchWithPlayerState(PlayerState& state)
{
	ChangeSelectedMenu();
}

void RespawnHud::ChangeSelectedMenu()
{
	for (int i = 0; i < m_numMenu; ++i)
	{
		bool on = false;
		if (i == m_selectedMenu) on = true;
		m_menu[i]->MatchWithData(PERVec2(0.2, -0.4 - i * 0.2), PERVec2(0.21, -0.4 - i * 0.2), on);
	}
}
