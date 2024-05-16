#include "stdafx.h"
#include "main_menu_hud.h"

MainMenuHud::MainMenuHud()
{
	// 게임 타이틀
	TextViewer* title = dynamic_cast<TextViewer*>(m_uiElementPool->Create(PERUiElementType::TEXT_VIEWER));
	if (title) title->Init(PERVec2(-0.8, 0.8), 0.3, PERColor(100, 0, 0), L"PERDITIO__");
	else PERLog::Logger().ErrorWithFormat("메인 메뉴 hud의 타이틀이 생성되지 않았습니다.");
}

MainMenuHud::~MainMenuHud()
{
}

void MainMenuHud::Recive(PEREvent event, PERVec3 data)
{
}

void MainMenuHud::MatchWithPlayerState(PlayerState& state)
{
}
