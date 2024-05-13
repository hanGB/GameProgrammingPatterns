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
#include "trigger_physics_component.h"
// graphics
#include "visible_graphics_component.h"
#include "name_tag_graphics_component.h"
#include "body_bar_graphics_componenet.h"
#include "key_input_helper_graphics_component.h"
#include "hidden_graphics_component.h"
#include "hidden_debug_graphics_component.h"

// 오브젝트 스테이트
#include "player_state.h"
#include "monster_state.h"


ObjectFactory::ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType, 
    std::vector<PERComponentType>& input, std::vector<PERComponentType>& ai, 
    std::vector<PERComponentType>& physics, std::vector<PERComponentType>& graphics)
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 생성", ( int ) objectType);

    m_objectType = objectType;
    m_objectStateType = objectStateType;
    m_componentTypeVectors = { input, ai, physics, graphics };
    
    MatchCollisionType();
    InitData();
}

ObjectFactory::ObjectFactory(PERObjectType objectType, PERObjectStateType objectStateType, 
    std::vector<PERComponentType>& input, std::vector<PERComponentType>& ai, 
    std::vector<PERComponentType>& physics, std::vector<PERComponentType>& graphics, 
    PERComponent::InputData& inputData, PERComponent::AiData& aiData, 
    PERComponent::PhysicsData& physicsData, PERComponent::GraphicsData& graphicsData)
{
    PERLog::Logger().InfoWithFormat("오브젝트 팩토리 object type(%d) 생성", ( int ) objectType);

    m_objectType = objectType;
    m_objectStateType = objectStateType;
    m_componentTypeVectors = { input, ai, physics, graphics };
    m_componentData = { inputData, aiData, physicsData, graphicsData };

    m_size = PERVec3(1.0, 1.0, 1.0); m_mass = 50.0;
    MatchCollisionType();
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
    InputComponent* inputComponent = CreateInputComponent(m_componentTypeVectors.input);
    AiComponent* aiComponent = CreateAiComponent(m_componentTypeVectors.ai);
    PhysicsComponent* physicsComponent = CreatePhysicsComponent(m_componentTypeVectors.physics);
    GraphicsComponent* graphicsComponent = CreateGraphicsComponent(m_componentTypeVectors.graphics);

    return new PERObject(*this, objectState, inputComponent, aiComponent, physicsComponent, graphicsComponent);
}


PERObjectType ObjectFactory::GetObjectType() const
{
    return m_objectType;
}

PERCollisionType ObjectFactory::GetCollisionType() const
{
    return m_collisionType;
}

PERComponent::ComponentTypeVectors& ObjectFactory::GetComponentTypeVectors()
{
    return m_componentTypeVectors;
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
            1, 50, 50, 20, 20, 10, 10
        };
        objectState->SetStat(m_stat);
        break;
    }
    case PERObjectStateType::MONSTER: {
        m_stat = {
            1, 50, 50, 15, 15, 5, 5
        };
        objectState->SetStat(m_stat);
        objectState->SetIsHasCollisionDamage(true);

        break;
    }
    case PERObjectStateType::NON: {
        objectState->SetIsImmortal(true);
        if ( m_objectType == PERObjectType::MOVABLE_BLOCK ) {
            objectState->SetNameId("OBJECT_BLOCK_NAME");
            objectState->SetIsImmortal(false);
            // 스텟을 최대 값으로 설정해 공격에 대해서는 웬만해서는 죽지 않도록 설정
            m_stat = {
                  PER_MAX_STAT, PER_MAX_STAT, PER_MAX_STAT,
                  PER_MAX_STAT, PER_MAX_STAT, PER_MAX_STAT, PER_MAX_STAT
            };
            objectState->SetStat(m_stat);

        }
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

InputComponent* ObjectFactory::CreateInputComponent(std::vector<PERComponentType>& types)
{
    if ( types.size() == 0 ) return nullptr;

    InputComponent* component = CreateInputComponent(*types.cbegin());

    if ( types.size() == 1 ) return component;

    InputComponent* frontComponent = component;
    for ( auto it = types.cbegin() + 1; it < types.cend(); ++it )
    {
        InputComponent* nextComponent = CreateInputComponent(*it);
        frontComponent->SetNextComponent(nextComponent);
        frontComponent = nextComponent;
    }

    return component;
}

AiComponent* ObjectFactory::CreateAiComponent(std::vector<PERComponentType>& types)
{
    if ( types.size() == 0 ) return nullptr;

    AiComponent* component = CreateAiComponent(*types.cbegin());

    if ( types.size() == 1 ) return component;

    AiComponent* frontComponent = component;
    for ( auto it = types.cbegin() + 1; it < types.cend(); ++it )
    {
        AiComponent* nextComponent = CreateAiComponent(*it);
        frontComponent->SetNextComponent(nextComponent);
        frontComponent = nextComponent;
    }

    return component;
}

PhysicsComponent* ObjectFactory::CreatePhysicsComponent(std::vector<PERComponentType>& types)
{
    if ( types.size() == 0 ) return nullptr;

    PhysicsComponent* component = CreatePhysicsComponent(*types.cbegin());

    if ( types.size() == 1 ) return component;

    PhysicsComponent* frontComponent = component;
    for ( auto it = types.cbegin() + 1; it < types.cend(); ++it )
    {
        PhysicsComponent* nextComponent = CreatePhysicsComponent(*it);
        frontComponent->SetNextComponent(nextComponent);
        frontComponent = nextComponent;
    }

    return component;
}

GraphicsComponent* ObjectFactory::CreateGraphicsComponent(std::vector<PERComponentType>& types)
{
    if ( types.size() == 0 ) return nullptr;

    GraphicsComponent* component = CreateGraphicsComponent(*types.cbegin());

    if ( types.size() == 1 ) return component;

    GraphicsComponent* frontComponent = component;
    for ( auto it = types.cbegin() + 1; it < types.cend(); ++it )
    {
        GraphicsComponent* nextComponent = CreateGraphicsComponent(*it);
        frontComponent->SetNextComponent(nextComponent);
        frontComponent = nextComponent;
    }

    return component;
}

InputComponent* ObjectFactory::CreateInputComponent(PERComponentType type)
{
    InputComponent* inputComponent = nullptr;
    switch ( type ) {
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

    return inputComponent;
}

AiComponent* ObjectFactory::CreateAiComponent(PERComponentType type)
{
    AiComponent* aiComponent = nullptr;
    switch ( type ) {
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
        break;
    }

    return aiComponent;
}

PhysicsComponent* ObjectFactory::CreatePhysicsComponent(PERComponentType type)
{
    PhysicsComponent* physicsComponent = nullptr;
    switch ( type ) {
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
    case PERComponentType::TRIGGER_PHYSICS:
        physicsComponent = new TriggerPhysicsComponent();
        break;
    }

    return physicsComponent;
}

GraphicsComponent* ObjectFactory::CreateGraphicsComponent(PERComponentType type)
{
    GraphicsComponent* graphicsComponent = nullptr;
    switch ( type ) {
    case PERComponentType::VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERComponentType::NAME_TAG_GRAPHICS:
        graphicsComponent = new NameTagGraphicsComponent();
        break;
    case PERComponentType::BODY_BAR_GRAPHICS:
        graphicsComponent = new BodyBarGraphicsComponent();
        break;
    case PERComponentType::KEY_INPUT_HELPER_GRAPHICS:
        graphicsComponent = new KeyInputHelperGraphicsComponent();
        break;
    case PERComponentType::HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    case PERComponentType::HIDDEN_DEBUG:
        graphicsComponent = new HiddenDebugGraphicsComponent();
        break;
    }

    return graphicsComponent;
}

void ObjectFactory::MatchCollisionType()
{
    switch (m_objectType) {
    case PERObjectType::PLAYER: m_collisionType = PERCollisionType::ACTOR;
        break;
    case PERObjectType::FIXED_BLOCK: m_collisionType = PERCollisionType::FIXED;
        break;
    case PERObjectType::MOVABLE_BLOCK: m_collisionType = PERCollisionType::MOVABLE;
        break;
    case PERObjectType::MONSTER: m_collisionType = PERCollisionType::ACTOR;
        break;
    case PERObjectType::BULLET: m_collisionType = PERCollisionType::BULLET;
        break;
    case PERObjectType::BLADE: m_collisionType = PERCollisionType::BULLET;
        break;
    case PERObjectType::SPAWNER: m_collisionType = PERCollisionType::NONE;
        break;
    case PERObjectType::BUTTON: m_collisionType = PERCollisionType::NONE;
        break;
    case PERObjectType::PRESSURE: m_collisionType = PERCollisionType::TRIGGER;
        break;
    case PERObjectType::DOOR: m_collisionType = PERCollisionType::FIXED;
        break;
    case PERObjectType::PARTICLE_EFFECTER: m_collisionType = PERCollisionType::NONE;
        break;
    case PERObjectType::TRIGGER: m_collisionType = PERCollisionType::TRIGGER;
        break;
    }
}
