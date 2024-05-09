#include "stdafx.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "per_game.h"
#include "game_state.h"
#include "object_factory.h"
#include "per_object.h"
#include "object_state.h"
#include "black_board.h"

GameMode::GameMode()
{

}

GameMode::~GameMode()
{
	delete m_hud;
	delete m_player;
    delete m_playerFactory;
	delete m_gameState;
}

void GameMode::StartUse()
{
    EventDispatcher::AddReciver(m_hud);
	EventDispatcher::AddReciver(&m_player->GetObjectState());
}

void GameMode::EndUse()
{
    EventDispatcher::RemoveReciver(m_hud);
	EventDispatcher::RemoveReciver(&m_player->GetObjectState());
}

PERHud& GameMode::GetHud()
{
	return *m_hud;
}

GameState& GameMode::GetGameState()
{
	return *m_gameState;
}

PERObject& GameMode::GetPlayer()
{
	return *m_player;
}

void GameMode::SetGameState(GameState* gameState)
{
	m_gameState = gameState;
}

void GameMode::InitGameMode()
{
	CreatePlayerFactory();
	m_player = m_playerFactory->CreateObject();
	m_hud = CreateHud();
}
