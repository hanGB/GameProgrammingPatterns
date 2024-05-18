#include "credits_game_mode.h"
#include "per_object.h"
#include "per_renderer.h"
#include "object_factory.h"
#include "black_board.h"
#include "parachute_falling_physics_component.h"

CreditsGameMode::CreditsGameMode()
{
    InitGameMode();
}

CreditsGameMode::~CreditsGameMode()
{
}

void CreditsGameMode::Update()
{
  
}

void CreditsGameMode::UpdateCamera(PERRenderer& renderer, double frameGap)
{
    PERVec3 pos = m_player->GetPosition();
    PERVec3 gap = m_player->GetVelocity() * frameGap * ((double)PER_MICROSEC_PER_UPDATE / 1'000'000.0);;

    renderer.SetCameraPosition(PERVec2(pos.x + gap.x, pos.y + gap.y));
}

void CreditsGameMode::InitGameMode()
{
    CreatePlayerFactory();
    m_player = m_playerFactory->CreateObject();

    ParachuteFallingPhysicsComponent* component = new ParachuteFallingPhysicsComponent();
    component->SetOwner(m_player);
    component->SetFallingSpeed(3.0);
    m_player->GetPhysics().SetNextComponent(component);
}

void CreditsGameMode::CreatePlayerFactory()
{
    PERComponent::InputData input;
    PERComponent::AiData ai;
    PERComponent::PhysicsData physics;
    PERComponent::GraphicsData graphics;

    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    std::vector<PERComponentType> inputTypes = { PERComponentType::NO_INTERACT };
    std::vector<PERComponentType> aiTypes = { PERComponentType::UNINTELLIGENT };
    std::vector<PERComponentType> physicsTypes = { PERComponentType::FIXED };
    std::vector<PERComponentType> graphicsTypes = { PERComponentType::HIDDEN };

    m_playerFactory = new ObjectFactory(
        PERObjectType::PLAYER, PERObjectStateType::PLAYER,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics
    );
    m_playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); m_playerFactory->SetMass(70);
}
