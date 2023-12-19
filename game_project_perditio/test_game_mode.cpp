#include "stdafx.h"
#include "test_game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "per_game.h"
#include "game_state.h"
#include "object_factory.h"
#include "per_object.h"
#include "object_state.h"
#include "black_board.h"

TestGameMode::TestGameMode(GameState* gameState)
{
    SetGameState(gameState);
    InitGameMode();
}

TestGameMode::~TestGameMode()
{
}

void TestGameMode::StartUse()
{
    GameMode::StartUse();
}

void TestGameMode::EndUse()
{
    GameMode::EndUse();
}

void TestGameMode::Update()
{
    BlackBoard::SetPlayerPos(m_player->GetPosition());
}

void TestGameMode::UpdateCamera(PERRenderer& renderer, double frameGap)
{
    PERVec3 pos = m_player->GetPosition();
    PERVec3 gap = m_player->GetVelocity() * frameGap * ((double)PER_MICROSEC_PER_UPDATE / 1'000'000.0);;

    renderer.SetCameraPosition(PERVec2(pos.x + gap.x, pos.y + gap.y));
}

void TestGameMode::CreatePlayerFactory()
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
    graphics.floatingUi = false; graphics.distanceVisiblefloatingUi = 0.0;
    m_playerFactory->SetInputData(input);             m_playerFactory->SetAiData(ai);
    m_playerFactory->SetPhysicsData(physics);         m_playerFactory->SetGraphicsData(graphics);
    m_playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); m_playerFactory->SetMass(70);
}

PERHud* TestGameMode::CreateHud()
{
    PERHud* hud = new PERHud();

    ObjectState playerState = m_player->GetObjectState();
    hud->GetBodyBar()->SetMax(playerState.GetStat().body);
    hud->GetBodyBar()->SetCurrent(playerState.GetCurrentBody());
    hud->GetMindBar()->SetMax(playerState.GetStat().mind);
    hud->GetMindBar()->SetCurrent(playerState.GetCurrentBody());

    return hud;
}

