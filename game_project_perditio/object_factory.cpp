#include "stdafx.h"
#include "object_factory.h"

// 각종 컨포넌트
// input
#include "player_input_component.h"
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

ObjectFactory::ObjectFactory(
    PERObjectType objectType, 
    PERComponentType input, PERComponentType ai, 
    PERComponentType physics, PERComponentType graphics)
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 생성", (int)objectType);

    m_objectType = objectType;
    m_componentTypes = { input, ai, physics, graphics };

    InitData();
}

ObjectFactory::~ObjectFactory()
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 삭제", (int)m_objectType);
}


PERObject* ObjectFactory::CreateObject()
{
    InputComponent* inputComponent = nullptr;
    switch (m_componentTypes.input) {
    case PERComponentType::PLAYER_INPUT:
        inputComponent = new PlayerInputComponent();
        break;
    case PERComponentType::INTERACT:
        inputComponent = new InteractInputComponent();
        break;
    case PERComponentType::NO_INTERACT:
        inputComponent = new NoInteractInputComponent();
        break;
    }

    AiComponent* aiComponent = nullptr;
    switch (m_componentTypes.ai) {
    case PERComponentType::UNINTELLIGENT:
        aiComponent = new UnintelligentAiComponent();
        break;
    case PERComponentType::INTELLIGENT:
        aiComponent = new IntelligentAiComponent();
        break;
    }

    PhysicsComponent* physicsComponent = nullptr;
    switch (m_componentTypes.physics) {
    case PERComponentType::MOVABLE:
        physicsComponent = new MovablePhysicsComponent();
        break;
    case PERComponentType::FIXED:
        physicsComponent = new FixedPhysicsComponent();
        break;
    }

    GraphicsComponent* graphicsComponent = nullptr;
    switch (m_componentTypes.graphics) {
    case PERComponentType::VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERComponentType::HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    }

    return new PERObject(*this, inputComponent, aiComponent, physicsComponent, graphicsComponent);
}


PERObjectType ObjectFactory::GetObjectType() const
{
    return m_objectType;
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
    m_componentData.graphics.shape = PERShapeType::RECTANGLE;
    m_componentData.graphics.color = PERColor(255, 255, 255);

    m_size = PERVec3(1.0, 1.0, 1.0);
    m_mass = 50.0;
}
