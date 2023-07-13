#include "stdafx.h"
#include "object_pool.h"

ObjectPool::ObjectPool()
{
    PERLog::Logger().Info("오브젝트 풀 생성");

    CreateObjectFactories();
    FillObjectPools();
}

ObjectPool::~ObjectPool()
{
    PERLog::Logger().Info("오브젝트 풀 삭제");

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
    // 플레이어는 게임 모드에서 생성하기 때문에 제외
    if (type == PERObjectType::PLAYER) return;

    m_objectPools.find(type)->second.push(object);
}

void ObjectPool::CreateObjectFactories()
{
    using namespace PERComponent;
    InputData input;
    AiData ai;
    PhysicsData physics;
    GraphicsData graphics;

    // block
    ObjectFactory* blockFactory
        = new ObjectFactory(
            PERObjectType::BLOCK,
            PERComponentType::NO_INTERACT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::FIXED,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(255, 255, 255);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(0, 0, 0);
    blockFactory->SetInputData(input);              blockFactory->SetAiData(ai);
    blockFactory->SetPhysicsData(physics);          blockFactory->SetGraphicsData(graphics);
    blockFactory->SetSize(PERVec3(1.0, 1.0, 1.0));  blockFactory->SetMass(100);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::BLOCK, blockFactory));

    // monster
    ObjectFactory* monsterFactory
        = new ObjectFactory(
            PERObjectType::MONSTER,
            PERComponentType::NO_INTERACT,
            PERComponentType::INTELLIGENT,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = true, ai.isMove = true;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::TRIANGLE; graphics.color = PERColor(255, 0, 0);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(127, 0, 0);
    monsterFactory->SetInputData(input);                monsterFactory->SetAiData(ai);
    monsterFactory->SetPhysicsData(physics);            monsterFactory->SetGraphicsData(graphics);
    monsterFactory->SetSize(PERVec3(0.25, 0.25, 0.25)); monsterFactory->SetMass(100);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::MONSTER, monsterFactory));

    // bullet
    ObjectFactory* bulletFactory
        = new ObjectFactory(
            PERObjectType::BULLET,
            PERComponentType::NO_INTERACT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false; 
    physics.friction = false; physics.isOccupySpace = false;
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(100, 100, 100);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(50, 50, 50);
    bulletFactory->SetInputData(input);                 bulletFactory->SetAiData(ai);
    bulletFactory->SetPhysicsData(physics);             bulletFactory->SetGraphicsData(graphics);
    bulletFactory->SetSize(PERVec3(0.2, 0.2, 0.2));  bulletFactory->SetMass(10);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::BULLET, bulletFactory));
}

void ObjectPool::DeleteObjectFactories()
{
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        auto it = m_objectFactories.find((PERObjectType)type);
        if (it == m_objectFactories.end()) continue;

        ObjectFactory* objectFactory = it->second;
        delete objectFactory;
    }
}

void ObjectPool::FillObjectPools()
{
    
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
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
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {

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
