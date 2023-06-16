#include "stdafx.h"
#include "object_factory.h"
// 각종 컨포넌트
// input
#include "player_input_component.h"
#include "interact_input_component.h"
#include "no_interact_input_component.h"
// ai
#include "player_ai_component.h"
#include "unintelligent_ai_component.h"
#include "intelligent_ai_component.h"
// physics
#include "player_physics_component.h"
#include "movable_physics_component.h"
#include "fixed_physics_component.h"
// graphics
#include "player_graphics_component.h"
#include "visible_graphics_component.h"
#include "hidden_graphics_component.h"


PERObject* ObjectFactory::CreatePlayer()
{
    return CreateObject(
        PERObjectType::OBJECT_TYPE_PLAYER,
        PERObjectType::OBJECT_TYPE_PLAYER,
        PERObjectType::OBJECT_TYPE_PLAYER,
        PERObjectType::OBJECT_TYPE_PLAYER
        );
}

PERObject* ObjectFactory::CreateWall()
{
    return CreateObject(
        PERObjectType::OBJECT_TYPE_NO_INTERACT,
        PERObjectType::OBJECT_TYPE_UNINTELLIGENT,
        PERObjectType::OBJECT_TYPE_FIXED,
        PERObjectType::OBJECT_TYPE_VISIBLE
    );
}

PERObject* ObjectFactory::CreateObject(PERObjectType inputType, PERObjectType aiType, PERObjectType physicsType, PERObjectType graphicsType)
{
    PERInputComponent* inputComponent = nullptr;
    switch (inputType) {
    case PERObjectType::OBJECT_TYPE_PLAYER:
        inputComponent = new PlayerInputComponent();
        break;
    case PERObjectType::OBJECT_TYPE_INTERACT:
        inputComponent = new InteractInputComponent();
        break;
    case PERObjectType::OBJECT_TYPE_NO_INTERACT:
        inputComponent = new NoInteractInputComponent();
        break;
    }

    PERAiComponent* aiComponent = nullptr;
    switch (aiType) {
    case PERObjectType::OBJECT_TYPE_PLAYER:
        aiComponent = new PlayerAiComponent();
        break;
    case PERObjectType::OBJECT_TYPE_UNINTELLIGENT:
        aiComponent = new UnintelligentAiComponent();
        break;
    case PERObjectType::OBJECT_TYPE_INTELLIGENT:
        aiComponent = new IntelligentAiComponent();
        break;
    }

    PERPhysicsComponent* physicsComponent = nullptr;
    switch (physicsType) {
    case PERObjectType::OBJECT_TYPE_PLAYER:
        physicsComponent = new PlayerPhysicsComponent();
        break;
    case PERObjectType::OBJECT_TYPE_MOVABLE:
        physicsComponent = new MovablePhysicsComponent();
        break;
    case PERObjectType::OBJECT_TYPE_FIXED:
        physicsComponent = new FixedPhysicsComponent();
        break;
    }

    PERGraphicsComponent* graphicsComponent = nullptr;
    switch (graphicsType) {
    case PERObjectType::OBJECT_TYPE_PLAYER:
        graphicsComponent = new PlayerGraphicsComponent();
        break;
    case PERObjectType::OBJECT_TYPE_VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERObjectType::OBJECT_TYPE_HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    }
 
    return new PERObject(inputComponent, aiComponent, physicsComponent, graphicsComponent);
}
