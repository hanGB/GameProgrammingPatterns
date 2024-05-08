#include "stdafx.h"
#include "object_factory.h"

// 각종 컨포넌트
// input
#include "player_input_component.h"
#include "interact_input_component.h"
#include "no_interact_input_component.h"
#include "button_input_component.h"
// ai
#include "unintelligent_ai_component.h"
#include "intelligent_ai_component.h"
#include "monster_ai_component.h"
#include "spawner_ai_component.h"
#include "making_signal_ai_component.h"
#include "response_to_signal_ai_component.h"
#include "creating_particles_ai_component.h"
// physics
#include "movable_physics_component.h"
#include "fixed_physics_component.h"
#include "stuck_physics_component.h"
#include "pressure_physics_componenet.h"
// graphics
#include "visible_graphics_component.h"
#include "visible_with_information_graphics_componenet.h"
#include "monster_graphics_componenet.h"
#include "button_graphics_component.h"
#include "hidden_graphics_component.h"

// 오브젝트 스테이트
#include "player_state.h"
#include "monster_state.h"


ObjectFactory::ObjectFactory(
    PERObjectType objectType, PERObjectStateType objectStateType,
    PERComponentType input, PERComponentType ai, 
    PERComponentType physics, PERComponentType graphics
    )
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 생성", (int)objectType);

    m_objectType = objectType;
    m_objectStateType = objectStateType;
    m_componentTypes = { input, ai, physics, graphics };

    InitData();
}

ObjectFactory::ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType,
    PERComponentType input, PERComponentType ai, PERComponentType physics, PERComponentType graphics, 
    PERComponent::InputData& inputData, PERComponent::AiData& aiData, 
    PERComponent::PhysicsData& physicsData, PERComponent::GraphicsData& graphicsData)
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 생성", (int)objectType);

    m_objectType = objectType;
    m_objectStateType = objectStateType;
    m_componentTypes = { input, ai, physics, graphics };
    m_componentData = { inputData, aiData, physicsData, graphicsData };

    m_size = PERVec3(1.0, 1.0, 1.0); m_mass = 50.0;
}

ObjectFactory::~ObjectFactory()
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 삭제", (int)m_objectType);
}


PERObject* ObjectFactory::CreateObject()
{
    // 오브젝트 스테이트 생성
    ObjectState* objectState = CreateObjectState();

    // 각 컴포넌트 타입 별로 생성
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
    case PERComponentType::BUTTON_INPUT:
        inputComponent = new ButtonInputComponent();
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
    case PERComponentType::MONSTER_AI:
        aiComponent = new MonsterAiComponent();
        break;
    case PERComponentType::SPAWNER_AI:
        aiComponent = new SpawnerAiComponent();
        break;
    case PERComponentType::MAKING_SIGNAL:
        aiComponent = new MakingSignalAiComponent();
        break;
    case PERComponentType::RESPONSE_TO_SIGNAL:
        aiComponent = new ResponeseToSignalAiComponent();
        break;
    case PERComponentType::CREATING_PARTICLE:
        aiComponent = new CreatingParticlesAiComponent();
    }

    PhysicsComponent* physicsComponent = nullptr;
    switch (m_componentTypes.physics) {
    case PERComponentType::MOVABLE:
        physicsComponent = new MovablePhysicsComponent();
        break;
    case PERComponentType::FIXED:
        physicsComponent = new FixedPhysicsComponent();
        break;
    case PERComponentType::STUCK:
        physicsComponent = new StuckPhysicsComponent();
        break;
    case PERComponentType::PRESSURE_PHYSICS:
        physicsComponent = new PressurePhysicsComponent();
        break;
    }

    GraphicsComponent* graphicsComponent = nullptr;
    switch (m_componentTypes.graphics) {
    case PERComponentType::VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERComponentType::VISIBLE_WITH_INFORMATION:
        graphicsComponent = new VisibleWithInformationGraphicsComponent();
        break;
    case PERComponentType::MONSTER_GRAPHICS:
        graphicsComponent = new MonsterGraphicsComponent();
        break;
    case PERComponentType::BUTTON_GRAPHICS:
        graphicsComponent = new ButtonGraphicsComponent();
        break;
    case PERComponentType::HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    }

    return new PERObject(*this, objectState, inputComponent, aiComponent, physicsComponent, graphicsComponent);
}


PERObjectType ObjectFactory::GetObjectType() const
{
    return m_objectType;
}

PERComponent::ComponentTypes ObjectFactory::GetComponentTypes() const
{
    return m_componentTypes;
}

void ObjectFactory::InitComponentDatas(
    PERComponent::InputData& input, PERComponent::AiData& ai, 
    PERComponent::PhysicsData& physics, PERComponent::GraphicsData& graphics)
{
    // input
    input.isAttack = false;
    input.isMove = false;
    input.isCheck = false;
    // ai
    ai.isAttack = false;
    ai.isMove = false;
    ai.isSwitch = false;
    ai.isDisposable = false;
    ai.particleDelay = 0.5;
    ai.particleLifeTime = 3.0;
    ai.particleEffectType = PERParticleEffectType::CIRCLE_BOMB;
    // physics
    physics.friction = true;
    physics.isOccupySpace = true;
    physics.stuckPosition = PERVec3(0.0, 0.0, 0.0);
    // graphics
    graphics.shape = PERShapeType::RECTANGLE;
    graphics.color = PERColor(255, 255, 255);
    graphics.border = true;
    graphics.borderColor = PERColor(0, 0, 0);
    graphics.borderWidth = 1;
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

void ObjectFactory::InitializeObjectState(ObjectState* objectState)
{
    objectState->Initialize();

    switch (m_objectStateType) {
    case PERObjectStateType::PLAYER: {
        m_stat = {
            1, 100, 100, 10, 10, 10, 10
        };
        objectState->SetStat(m_stat);
        break;
    }
    case PERObjectStateType::MONSTER: {
        m_stat = {
            1, 50, 50, 5, 5, 5, 5
        };
        objectState->SetStat(m_stat);
        objectState->SetIsHasCollisionDamage(true);

        break;
    }
    case PERObjectStateType::NON: {
        objectState->SetIsImmortal(true);

        if (m_objectType == PERObjectType::MOVABLE_BLOCK)  objectState->SetNameId("OBJECT_BLOCK_NAME");
        else if (m_objectType == PERObjectType::BUTTON)  objectState->SetNameId("OBJECT_BUTTON_NAME");
        else if (m_objectType == PERObjectType::PRESSURE)  objectState->SetNameId("OBJECT_PRESSURE_PLATE_NAME");
        else if (m_objectType == PERObjectType::DOOR)  objectState->SetNameId("OBJECT_DOOR_NAME");

        break;
    }
    }
}

void ObjectFactory::InitData()
{
    InitComponentDatas(m_componentData.input, m_componentData.ai, m_componentData.physics, m_componentData.graphics);

    m_size = PERVec3(1.0, 1.0, 1.0);
    m_mass = 50.0;
}

ObjectState* ObjectFactory::CreateObjectState()
{
    ObjectState* objectState = nullptr;

    switch (m_objectStateType) {
    case PERObjectStateType::PLAYER: {
        objectState = new PlayerState();
        break;
    }
    case PERObjectStateType::MONSTER: {
        objectState = new MonsterState();
        break;
    }
    case PERObjectStateType::NON: {
        objectState = new ObjectState();
        break;
    }
    }
    InitializeObjectState(objectState);

    return objectState;
}
