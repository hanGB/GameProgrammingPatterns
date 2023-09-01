#include "stdafx.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "per_game.h"
#include "game_state.h"
#include "object_factory.h"
#include "per_object.h"
#include "object_state.h"

GameMode::GameMode()
{
    CreatePlayerFactory();

	m_hud = new PERHud();
	m_gameState = new GameState();

	m_player = m_playerFactory->CreateObject();

    // hud ¼³Á¤
    ObjectState playerState = m_player->GetObjectState();
    m_hud->GetBodyBar()->SetMax(playerState.GetStat().body);
    m_hud->GetBodyBar()->SetCurrent(playerState.GetCurrentBody());
    m_hud->GetMindBar()->SetMax(playerState.GetStat().mind);
    m_hud->GetMindBar()->SetCurrent(playerState.GetCurrentBody());
}

GameMode::~GameMode()
{
	delete m_hud;
	delete m_gameState;
	delete m_player;

    delete m_playerFactory;
}

void GameMode::StartUse()
{
    EventDispatcher::AddReciver(m_hud);
}

void GameMode::EndUse()
{
    EventDispatcher::RemoveReciver(m_hud);
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

void GameMode::CreatePlayerFactory()
{
    PERComponent::InputData input;
    PERComponent::AiData ai;
    PERComponent::PhysicsData physics;
    PERComponent::GraphicsData graphics;

    m_playerFactory = new ObjectFactory(
            PERObjectType::PLAYER,
            PERObjectStateType::PLAYER,
            PERFloatingUiType::NON,
            PERComponentType::PLAYER_INPUT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = true, input.isMove = true, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(0, 255, 255);
    graphics.border = true; graphics.borderWidth = 3; graphics.borderColor = PERColor(0, 127, 127);
    m_playerFactory->SetInputData(input);             m_playerFactory->SetAiData(ai);
    m_playerFactory->SetPhysicsData(physics);         m_playerFactory->SetGraphicsData(graphics);
    m_playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); m_playerFactory->SetMass(50);
}
