#include "credits_game_mode.h"
#include "per_object.h"
#include "per_renderer.h"
#include "object_factory.h"
#include "black_board.h"

CreditsGameMode::CreditsGameMode()
{
    InitGameMode();
}

CreditsGameMode::~CreditsGameMode()
{
}

void CreditsGameMode::Update()
{
    BlackBoard::SetPlayerPos(m_player->GetPosition());
}

void CreditsGameMode::UpdateCamera(PERRenderer& renderer, double frameGap)
{
    PERVec3 pos = m_player->GetPosition();
    PERVec3 gap = m_player->GetVelocity() * frameGap * ((double)PER_MICROSEC_PER_UPDATE / 1'000'000.0);;

    renderer.SetCameraPosition(PERVec2(pos.x + gap.x, pos.y + gap.y));
}

void CreditsGameMode::CreatePlayerFactory()
{
    PERComponent::InputData input;
    PERComponent::AiData ai;
    PERComponent::PhysicsData physics;
    PERComponent::GraphicsData graphics;

    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(100, 100, 100);

    std::vector<PERComponentType> inputTypes = { PERComponentType::PLAYER_INPUT };
    std::vector<PERComponentType> aiTypes = { PERComponentType::UNINTELLIGENT };
    std::vector<PERComponentType> physicsTypes = { PERComponentType::MOVABLE };
    std::vector<PERComponentType> graphicsTypes = { PERComponentType::VISIBLE };

    m_playerFactory = new ObjectFactory(
        PERObjectType::PLAYER, PERObjectStateType::PLAYER,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics
    );
    m_playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); m_playerFactory->SetMass(70);
}
