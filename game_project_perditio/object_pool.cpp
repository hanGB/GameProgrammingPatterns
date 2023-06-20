#include "stdafx.h"
#include "object_pool.h"

ObjectPool::ObjectPool()
{
    CreateObjectFactories();
    FillObjectPools();
}

ObjectPool::~ObjectPool()
{
    ClearObjectPools();
    DeleteObjectFactories();
}

PERObject* ObjectPool::PopObject(PERObjectType type)
{
    PERObject* object = m_objectPools.find(type)->second.front();
    m_objectPools.find(type)->second.pop();
    return object;
}

void ObjectPool::PushObject(PERObjectType type, PERObject* object)
{
    m_objectPools.find(type)->second.push(object);
}

void ObjectPool::CreateObjectFactories()
{
    using namespace PERComponent;
    InputData input;
    AiData ai;
    PhysicsData physics;
    GraphicsData graphics;

    // player
    ObjectFactory* playerFactory
        = new ObjectFactory(
            PERObjectType::OBJECT_TYPE_PLAYER,
            PERComponentType::COMPONENT_TYPE_INTERACT,
            PERComponentType::COMPONENT_TYPE_UNINTELLIGENT,
            PERComponentType::COMPONENT_TYPE_MOVABLE,
            PERComponentType::COMPONENT_TYPE_VISIBLE
        );
    input.isAttack = true, input.isMove = true, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true;
    graphics.shape = PERShapeType::SHAPE_TYPE_ELLIPSE; graphics.color = PERColor(0, 255, 255);
    playerFactory->SetInputData(input); playerFactory->SetAiData(ai);
    playerFactory->SetPhysicsData(physics); playerFactory->SetGraphicsData(graphics);
    playerFactory->SetSize(PERVec3(0.5, 0.5, 0.5)); playerFactory->SetMass(50);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::OBJECT_TYPE_PLAYER, playerFactory));

    // block
    ObjectFactory* blockFactory
        = new ObjectFactory(
            PERObjectType::OBJECT_TYPE_BLOCK,
            PERComponentType::COMPONENT_TYPE_NO_INTERACT,
            PERComponentType::COMPONENT_TYPE_UNINTELLIGENT,
            PERComponentType::COMPONENT_TYPE_FIXED,
            PERComponentType::COMPONENT_TYPE_VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true;
    graphics.shape = PERShapeType::SHAPE_TYPE_RECTANGLE; graphics.color = PERColor(255, 255, 255);
    blockFactory->SetInputData(input); blockFactory->SetAiData(ai);
    blockFactory->SetPhysicsData(physics); blockFactory->SetGraphicsData(graphics);
    blockFactory->SetSize(PERVec3(1.0, 1.0, 1.0)); blockFactory->SetMass(100);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::OBJECT_TYPE_BLOCK, blockFactory));
}

void ObjectPool::DeleteObjectFactories()
{
    for (int type = (int)PERObjectType::OBJECT_TYPE_PLAYER; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        auto it = m_objectFactories.find((PERObjectType)type);
        if (it == m_objectFactories.end()) continue;

        ObjectFactory* objectFactory = it->second;
        delete objectFactory;
    }
}

void ObjectPool::FillObjectPools()
{
    
    for (int type = (int)PERObjectType::OBJECT_TYPE_PLAYER; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        std::queue<PERObject*> pool;
        
        auto it = m_objectFactories.find((PERObjectType)type);
        if (it == m_objectFactories.end()) continue;

        ObjectFactory* objectFactory = it->second;
        for (int i = 0; i < PER_DEFAULT_OBJECT_POOL_SIZE; ++i) {    
            pool.push(objectFactory->CreateObject());
        }
        m_objectPools.insert(std::pair<PERObjectType, std::queue<PERObject*>>((PERObjectType)type, pool));
    }
}

void ObjectPool::ClearObjectPools()
{
    for (int type = (int)PERObjectType::OBJECT_TYPE_PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {

        auto it = m_objectPools.find((PERObjectType)type);
        if (it == m_objectPools.end()) continue;

        std::queue<PERObject*> pool = it->second;
        while (!pool.empty()) {
            PERObject* object = pool.front();
            pool.pop();
            delete object;
        }
    }

}
