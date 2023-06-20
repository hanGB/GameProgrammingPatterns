#include "stdafx.h"
#include "object_factory.h"

// 각종 컨포넌트
// input
#include "interact_input_component.h"
#include "no_interact_input_component.h"
// ai
#include "unintelligent_ai_component.h"
#include "intelligent_ai_component.h"
// physics
#include "movable_physics_component.h"
#include "fixed_physics_component.h"
// graphics
#include "visible_graphics_component.h"
#include "hidden_graphics_component.h"

ObjectFactory::ObjectFactory(PERComponentType input, PERComponentType ai, PERComponentType physics, PERComponentType graphics)
{
    m_componentTypes = { input, ai, physics, graphics };

    InitData();
}

ObjectFactory::~ObjectFactory()
{

}


PERObject* ObjectFactory::CreateObject()
{
    PERInputComponent* inputComponent = nullptr;
    switch (m_componentTypes.input) {
    case PERComponentType::COMPONENT_TYPE_INTERACT:
        inputComponent = new InteractInputComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_NO_INTERACT:
        inputComponent = new NoInteractInputComponent();
        break;
    }

    PERAiComponent* aiComponent = nullptr;
    switch (m_componentTypes.ai) {
    case PERComponentType::COMPONENT_TYPE_UNINTELLIGENT:
        aiComponent = new UnintelligentAiComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_INTELLIGENT:
        aiComponent = new IntelligentAiComponent();
        break;
    }

    PERPhysicsComponent* physicsComponent = nullptr;
    switch (m_componentTypes.physics) {
    case PERComponentType::COMPONENT_TYPE_MOVABLE:
        physicsComponent = new MovablePhysicsComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_FIXED:
        physicsComponent = new FixedPhysicsComponent();
        break;
    }

    PERGraphicsComponent* graphicsComponent = nullptr;
    switch (m_componentTypes.graphics) {
    case PERComponentType::COMPONENT_TYPE_VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    }

    return new PERObject(*this, inputComponent, aiComponent, physicsComponent, graphicsComponent);
}


PERComponent::ComponentTypes ObjectFactory::GetComponentTypes() const
{
    return m_componentTypes;
}

void ObjectFactory::SetInputData(PERComponent::InputData input)
{
    m_componentData.input = input;
}

void ObjectFactory::SetAiData(PERComponent::AiData ai)
{
    m_componentData.ai = ai;
}

void ObjectFactory::SetPhysicsData(PERComponent::PhysicsData physics)
{
    m_componentData.physics = physics;
}

void ObjectFactory::SetGraphicsData(PERComponent::GraphicsData graphics)
{
    m_componentData.graphics = graphics;
}

void ObjectFactory::SetSize(PERVec3 size)
{
    m_size = size;
}

void ObjectFactory::SetMass(double mass)
{
    m_mass = mass;
}

PERComponent::InputData ObjectFactory::GetInputData() const
{
    return m_componentData.input;
}

PERComponent::AiData ObjectFactory::GetAiData() const
{
    return m_componentData.ai;
}

PERComponent::PhysicsData ObjectFactory::GetPhysicsData() const
{
    return m_componentData.physics;
}

PERComponent::GraphicsData ObjectFactory::GetGraphicsData() const
{
    return m_componentData.graphics;
}

PERVec3 ObjectFactory::GetSize() const
{
    return m_size;
}

double ObjectFactory::GetMass() const
{
    return m_mass;
}

void ObjectFactory::InitData()
{
    // 컨포넌트 데이터 초기화
    // input
    m_componentData.input.isAttack = false;
    m_componentData.input.isMove = false;
    m_componentData.input.isCheck = false;
    // ai
    m_componentData.ai.isAttack = false;
    m_componentData.ai.isMove = false;
    // physics
    m_componentData.physics.friction = true;
    // graphics
    m_componentData.graphics.shape = PERShapeType::SHAPE_TYPE_RECTANGLE;
    m_componentData.graphics.color = PERColor(255, 255, 255);

    m_size = PERVec3(1.0, 1.0, 1.0);
    m_mass = 50.0;
}
