#include "stdafx.h"
#include "menu_game_mode.h"
#include "per_component.h"
#include "object_factory.h"

MenuGameMode::MenuGameMode()
{
	InitGameMode();
}

MenuGameMode::~MenuGameMode()
{
}

void MenuGameMode::Update()
{

}

void MenuGameMode::UpdateCamera(PERRenderer& renderer, double frameGap)
{
    
}

void MenuGameMode::CreatePlayerFactory()
{
    std::vector<PERComponentType> inputTypes = { PERComponentType::MENU_SELECT};
    std::vector<PERComponentType> aiTypes = { PERComponentType::UNINTELLIGENT };
    std::vector<PERComponentType> physicsTypes = { PERComponentType::FIXED };
    std::vector<PERComponentType> graphicsTypes = { PERComponentType::HIDDEN };

    m_playerFactory = new ObjectFactory(
        PERObjectType::PLAYER, PERObjectStateType::PLAYER,
        inputTypes, aiTypes, physicsTypes, graphicsTypes
    );
    m_playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); m_playerFactory->SetMass(70);
}
