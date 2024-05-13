#include "stdafx.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "per_game.h"
#include "game_state.h"
#include "object_factory.h"
#include "per_object.h"
#include "player_state.h"
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
	EventDispatcher::AddReciver(&GetPlayerState());
	GetPlayerState().MatchDataAndHud();
	BlackBoard::SetIsPlayerLiving(true);
}

void GameMode::EndUse()
{
    EventDispatcher::RemoveReciver(m_hud);
	EventDispatcher::RemoveReciver(&GetPlayerState());
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

PlayerState& GameMode::GetPlayerState()
{
	return dynamic_cast<PlayerState&>(m_player->GetObjectState());
}

void GameMode::SetGameState(GameState* gameState)
{
	m_gameState = gameState;
}

void GameMode::UpdatePlayerState(PlayerState* updatedState)
{
	PlayerState* state = dynamic_cast<PlayerState*>(&m_player->GetObjectState());
	state->CopyData(updatedState);
}

void GameMode::InitGameMode()
{
	CreatePlayerFactory();
	m_player = m_playerFactory->CreateObject();
	m_hud = CreateHud();
}
