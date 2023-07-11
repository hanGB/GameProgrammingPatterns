#include "stdafx.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "per_game.h"

GameMode::GameMode()
{
	m_hud = new PERHud();
	m_player = PERGame::Instance().CreateObject(PERObjectType::OBJECT_TYPE_PLAYER);
}

GameMode::~GameMode()
{
	EventDispatcher::RemoveReciver(m_hud);
	delete m_hud;
}

void GameMode::Init()
{
	EventDispatcher::AddReciver(m_hud);
}

PERObject* GameMode::GetPlayer()
{
	return m_player;
}

PERHud* GameMode::GetHud()
{
	return m_hud;
}
