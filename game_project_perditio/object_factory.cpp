#include "stdafx.h"
#include "object_factory.h"

ObjectFactory::ObjectFactory()
{
    FillObjectPool();
}

ObjectFactory::~ObjectFactory()
{
    ClearObjectPool();
}

PERObject* ObjectFactory::PopObjectInPool(PERObjectType type)
{
    PERObject* object = m_objectPools.find(type)->second.front();
    m_objectPools.find(type)->second.pop();
    return object;
}

void ObjectFactory::PushObjectInPool(PERObjectType type, PERObject* object)
{
    m_objectPools.find(type)->second.push(object);
}

void ObjectFactory::FillObjectPool()
{
    struct componentTypes {
        PERComponentType input;
        PERComponentType ai;
        PERComponentType physics;
        PERComponentType graphics;
    };

    componentTypes data[(int)PERObjectType::NUM_OBJECT_TYPE];
    data[(int)PERObjectType::OBJECT_TYPE_PLAYER].input = PERComponentType::COMPONENT_TYPE_INTERACT;
    data[(int)PERObjectType::OBJECT_TYPE_PLAYER].ai = PERComponentType::COMPONENT_TYPE_UNINTELLIGENT;
    data[(int)PERObjectType::OBJECT_TYPE_PLAYER].physics = PERComponentType::COMPONENT_TYPE_MOVABLE;
    data[(int)PERObjectType::OBJECT_TYPE_PLAYER].graphics = PERComponentType::COMPONENT_TYPE_VISIBLE;

    data[(int)PERObjectType::OBJECT_TYPE_BLOCK].input = PERComponentType::COMPONENT_TYPE_NO_INTERACT;
    data[(int)PERObjectType::OBJECT_TYPE_BLOCK].ai = PERComponentType::COMPONENT_TYPE_UNINTELLIGENT;
    data[(int)PERObjectType::OBJECT_TYPE_BLOCK].physics = PERComponentType::COMPONENT_TYPE_FIXED;
    data[(int)PERObjectType::OBJECT_TYPE_BLOCK].graphics = PERComponentType::COMPONENT_TYPE_VISIBLE;

    for (int type = (int)PERObjectType::OBJECT_TYPE_PLAYER; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        std::queue<PERObject*> pool;
        for (int i = 0; i < PER_DEFAULT_OBJECT_POOL_SIZE; ++i) {
            pool.push(CreateObject(data[type].input, data[type].ai, data[type].physics, data[type].graphics));
        }
        m_objectPools.insert(std::pair<PERObjectType, std::queue<PERObject*>>((PERObjectType)type, pool));
    }
}

void ObjectFactory::ClearObjectPool()
{
    for (int type = (int)PERObjectType::OBJECT_TYPE_PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        std::queue<PERObject*> pool = m_objectPools.find((PERObjectType)type)->second;
        while (!pool.empty()) {
            PERObject* object = pool.front();
            pool.pop();
            delete object;
        }
    }
  
}

PERObject* ObjectFactory::CreateObject(PERComponentType inputType, PERComponentType aiType, PERComponentType physicsType, PERComponentType graphicsType)
{
    PERInputComponent* inputComponent = nullptr;
    switch (inputType) {
    case PERComponentType::COMPONENT_TYPE_INTERACT:
        inputComponent = new InteractInputComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_NO_INTERACT:
        inputComponent = new NoInteractInputComponent();
        break;
    }

    PERAiComponent* aiComponent = nullptr;
    switch (aiType) {
    case PERComponentType::COMPONENT_TYPE_UNINTELLIGENT:
        aiComponent = new UnintelligentAiComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_INTELLIGENT:
        aiComponent = new IntelligentAiComponent();
        break;
    }

    PERPhysicsComponent* physicsComponent = nullptr;
    switch (physicsType) {
    case PERComponentType::COMPONENT_TYPE_MOVABLE:
        physicsComponent = new MovablePhysicsComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_FIXED:
        physicsComponent = new FixedPhysicsComponent();
        break;
    }

    PERGraphicsComponent* graphicsComponent = nullptr;
    switch (graphicsType) {
    case PERComponentType::COMPONENT_TYPE_VISIBLE:
        graphicsComponent = new VisibleGraphicsComponent();
        break;
    case PERComponentType::COMPONENT_TYPE_HIDDEN:
        graphicsComponent = new HiddenGraphicsComponent();
        break;
    }
 
    return new PERObject(inputComponent, aiComponent, physicsComponent, graphicsComponent);
}
