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

    // fixed block
    ObjectFactory* fixedBlockFactory
        = new ObjectFactory(
            PERObjectType::FIXED_BLOCK,
            PERObjectStateType::NON,
            PERFloatingUiType::NON,
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
    graphics.floatingUi = false; graphics.distanceVisiblefloatingUi = 0.0;
    fixedBlockFactory->SetInputData(input);              fixedBlockFactory->SetAiData(ai);
    fixedBlockFactory->SetPhysicsData(physics);          fixedBlockFactory->SetGraphicsData(graphics);
    fixedBlockFactory->SetSize(PERVec3(1.0, 1.0, 1.0));  fixedBlockFactory->SetMass(100'000);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::FIXED_BLOCK, fixedBlockFactory));

    // movable block
    ObjectFactory* movableBlockFactory
        = new ObjectFactory(
            PERObjectType::MOVABLE_BLOCK,
            PERObjectStateType::NON,
            PERFloatingUiType::NON,
            PERComponentType::NO_INTERACT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(255, 255, 255);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(0, 255, 0);
    graphics.floatingUi = false; graphics.distanceVisiblefloatingUi = 0.0;
    movableBlockFactory->SetInputData(input);              movableBlockFactory->SetAiData(ai);
    movableBlockFactory->SetPhysicsData(physics);          movableBlockFactory->SetGraphicsData(graphics);
    movableBlockFactory->SetSize(PERVec3(1.0, 1.0, 1.0));  movableBlockFactory->SetMass(10);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::MOVABLE_BLOCK, movableBlockFactory));

    // monster
    ObjectFactory* monsterFactory
        = new ObjectFactory(
            PERObjectType::MONSTER,
            PERObjectStateType::MONSTER,
            PERFloatingUiType::PROGRESS_BAR,
            PERComponentType::NO_INTERACT,
            PERComponentType::MONSTER_AI,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = true, ai.isMove = true;
    physics.friction = true; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::TRIANGLE; graphics.color = PERColor(255, 0, 0);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(127, 0, 0);
    graphics.floatingUi = true; graphics.distanceVisiblefloatingUi = 100.0;
    monsterFactory->SetInputData(input);                monsterFactory->SetAiData(ai);
    monsterFactory->SetPhysicsData(physics);            monsterFactory->SetGraphicsData(graphics);
    monsterFactory->SetSize(PERVec3(0.25, 0.25, 0.25)); monsterFactory->SetMass(30);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::MONSTER, monsterFactory));

    // bullet
    ObjectFactory* bulletFactory
        = new ObjectFactory(
            PERObjectType::BULLET,
            PERObjectStateType::NON,
            PERFloatingUiType::NON,
            PERComponentType::NO_INTERACT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::MOVABLE,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false; 
    physics.friction = false; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(100, 100, 100);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(50, 50, 50);
    graphics.floatingUi = false; graphics.distanceVisiblefloatingUi = 0.0;
    bulletFactory->SetInputData(input);                 bulletFactory->SetAiData(ai);
    bulletFactory->SetPhysicsData(physics);             bulletFactory->SetGraphicsData(graphics);
    bulletFactory->SetSize(PERVec3(0.2, 0.2, 0.2));  bulletFactory->SetMass(5);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::BULLET, bulletFactory));

    // blade
    ObjectFactory* bladeFactory
        = new ObjectFactory(
            PERObjectType::BLADE,
            PERObjectStateType::NON,
            PERFloatingUiType::NON,
            PERComponentType::NO_INTERACT,
            PERComponentType::UNINTELLIGENT,
            PERComponentType::STUCK,
            PERComponentType::VISIBLE
        );
    input.isAttack = false, input.isMove = false, input.isCheck = false;
    ai.isAttack = false, ai.isMove = false;
    physics.friction = false; physics.isOccupySpace = true;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(200, 200, 200);
    graphics.border = true; graphics.borderWidth = 1; graphics.borderColor = PERColor(50, 50, 50);
    graphics.floatingUi = false; graphics.distanceVisiblefloatingUi = 0.0;
    bladeFactory->SetInputData(input);                bladeFactory->SetAiData(ai);
    bladeFactory->SetPhysicsData(physics);            bladeFactory->SetGraphicsData(graphics);
    bladeFactory->SetSize(PERVec3(0.5, 0.5, 0.5));    bladeFactory->SetMass(10);
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(PERObjectType::BLADE, bladeFactory));
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
