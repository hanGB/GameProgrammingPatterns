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
    auto& pool = m_objectPools.find(type)->second;

    if (pool.empty()) RefillObjectPool(pool, type);

    PERObject* object = pool.front();
    pool.pop();
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
            PERComponentType::COMPONENT_TYPE_PLAYER_INPUT,
            PERComponentType::COMPONENT_TYPE_UNINTELLIGENT,
            PERComponentType::COMPONENT_TYPE_MOVABLE,
            PERComponentType::COMPONENT_TYPE_VISIBLE
        );
    input.isAttack = true, input.isMove = true, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::SHAPE_TYPE_ELLIPSE; graphics.color = PERColor(0, 255, 255);
    graphics.border = true; graphics.borderWidth = 3; graphics.borderColor = PERColor(0, 127, 127);
    playerFactory->SetInputData(input);             playerFactory->SetAiData(ai);
    playerFactory->SetPhysicsData(physics);         playerFactory->SetGraphicsData(graphics);
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
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::SHAPE_TYPE_RECTANGLE; graphics.color = PERColor(255, 255, 255);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(0, 0, 0);
    blockFactory->SetInputData(input);              blockFactory->SetAiData(ai);
    blockFactory->SetPhysicsData(physics);          blockFactory->SetGraphicsData(graphics);
    blockFactory->SetSize(PERVec3(1.0, 1.0, 1.0));  blockFactory->SetMass(100);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::OBJECT_TYPE_BLOCK, blockFactory));

    // monster
    ObjectFactory* monsterFactory
        = new ObjectFactory(
            PERObjectType::OBJECT_TYPE_MONSTER,
            PERComponentType::COMPONENT_TYPE_NO_INTERACT,
            PERComponentType::COMPONENT_TYPE_INTELLIGENT,
            PERComponentType::COMPONENT_TYPE_MOVABLE,
            PERComponentType::COMPONENT_TYPE_VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = true, ai.isMove = true;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::SHAPE_TYPE_TRIANGLE; graphics.color = PERColor(255, 0, 0);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(127, 0, 0);
    monsterFactory->SetInputData(input);                monsterFactory->SetAiData(ai);
    monsterFactory->SetPhysicsData(physics);            monsterFactory->SetGraphicsData(graphics);
    monsterFactory->SetSize(PERVec3(0.25, 0.25, 0.25)); monsterFactory->SetMass(100);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::OBJECT_TYPE_MONSTER, monsterFactory));

    // bullet
    ObjectFactory* bulletFactory
        = new ObjectFactory(
            PERObjectType::OBJECT_TYPE_BULLET,
            PERComponentType::COMPONENT_TYPE_NO_INTERACT,
            PERComponentType::COMPONENT_TYPE_UNINTELLIGENT,
            PERComponentType::COMPONENT_TYPE_MOVABLE,
            PERComponentType::COMPONENT_TYPE_VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false; 
    physics.friction = false; physics.isOccupySpace = false;
    graphics.shape = PERShapeType::SHAPE_TYPE_ELLIPSE; graphics.color = PERColor(100, 100, 100);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(50, 50, 50);
    bulletFactory->SetInputData(input);                 bulletFactory->SetAiData(ai);
    bulletFactory->SetPhysicsData(physics);             bulletFactory->SetGraphicsData(graphics);
    bulletFactory->SetSize(PERVec3(0.2, 0.2, 0.2));  bulletFactory->SetMass(10);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::OBJECT_TYPE_BULLET, bulletFactory));
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

void ObjectPool::RefillObjectPool(std::queue<PERObject*>& pool, PERObjectType type)
{
    auto it = m_objectFactories.find(type);
    if (it == m_objectFactories.end()) return;

    ObjectFactory* objectFactory = it->second;
    for (int i = 0; i < PER_DEFAULT_OBJECT_POOL_SIZE; ++i) {
        pool.push(objectFactory->CreateObject());
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
