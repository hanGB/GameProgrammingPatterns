#include "stdafx.h"
#include "object_storage.h"

ObjectStorage::ObjectStorage()
{
    PERLog::Logger().Info("오브젝트 풀 생성");

    CreateObjectFactories();
    FillObjectQueues();
}

ObjectStorage::~ObjectStorage()
{
    PERLog::Logger().Info("오브젝트 풀 삭제");

    ClearObjectQueues();
    DeleteObjectFactories();
}

PERObject* ObjectStorage::PopObject(PERObjectType type)
{
    auto& queue = m_objectQueues.find(type)->second;

    if (queue.empty()) RefillObjectQueue(queue, type);

    PERObject* object = queue.front();
    queue.pop();
    return object;
}

void ObjectStorage::PushObject(PERObjectType type, PERObject* object)
{
    // 플레이어는 게임 모드에서 생성하기 때문에 제외
    if (type == PERObjectType::PLAYER) return;

    m_objectQueues.find(type)->second.push(object);
}

void ObjectStorage::CreateObjectFactories()
{
    ObjectFactory* factory;
    InputData input; AiData ai; PhysicsData physics; GraphicsData graphics;
   
    // fixed block
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);

    factory = CreateObjectFactory(PERObjectType::FIXED_BLOCK, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::UNINTELLIGENT, PERComponentType::FIXED, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));  factory->SetMass(100'000);

    // movable block
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.borderColor = PERColor(0, 255, 0);
    
    factory = CreateObjectFactory(PERObjectType::MOVABLE_BLOCK, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::UNINTELLIGENT, PERComponentType::MOVABLE, PERComponentType::VISIBLE,
        input, ai, physics, graphics);
    
    factory->SetSize(PERVec3(1.0, 1.0, 1.0));  factory->SetMass(50);

    // monster
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.shape = PERShapeType::TRIANGLE; graphics.color = PERColor(255, 0, 0);
    graphics.borderColor = PERColor(127, 0, 0);

    factory = CreateObjectFactory(PERObjectType::MONSTER, PERObjectStateType::MONSTER,
        PERComponentType::NO_INTERACT, PERComponentType::MONSTER_AI, PERComponentType::MOVABLE, PERComponentType::MONSTER_GRAPHICS,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.25, 0.25, 0.25)); factory->SetMass(30);

    // bullet
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.friction = false;
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(100, 100, 100);
    graphics.borderColor = PERColor(50, 50, 50);
   
    factory = CreateObjectFactory(PERObjectType::BULLET, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::UNINTELLIGENT, PERComponentType::MOVABLE, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.2, 0.2, 0.2));     factory->SetMass(5);

    // blade
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics); 
    physics.friction = false; 
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(200, 200, 200);
    graphics.borderColor = PERColor(50, 50, 50);
  
    factory = CreateObjectFactory(PERObjectType::BLADE, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::UNINTELLIGENT, PERComponentType::STUCK, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.5, 0.5, 0.5));    factory->SetMass(10);

    // spawner
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.isOccupySpace = false;

    factory = CreateObjectFactory(PERObjectType::SPAWNER, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::SPAWNER_AI, PERComponentType::FIXED, PERComponentType::HIDDEN,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));    factory->SetMass(100);

    // button
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.isOccupySpace = false;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(100, 100, 100);

    factory = CreateObjectFactory(PERObjectType::BUTTON, PERObjectStateType::NON,
        PERComponentType::BUTTON_INPUT, PERComponentType::MAKING_SIGNAL, PERComponentType::FIXED, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.4, 0.8, 0.5));    factory->SetMass(50);

    // pressure
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    ai.isSwitch = true;
    physics.isOccupySpace = false;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(150, 150, 150);

    factory = CreateObjectFactory(PERObjectType::PRESSURE, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::MAKING_SIGNAL, PERComponentType::PRESSURE_PHYSICS, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));    factory->SetMass(100);

    // door
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(100, 50, 50);

    factory = CreateObjectFactory(PERObjectType::DOOR, PERObjectStateType::NON,
        PERComponentType::NO_INTERACT, PERComponentType::RESPONSE_TO_SIGNAL, PERComponentType::FIXED, PERComponentType::VISIBLE,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(2.0, 1.0, 1.0));    factory->SetMass(100);
}

ObjectFactory* ObjectStorage::CreateObjectFactory(
    PERObjectType objectType, PERObjectStateType objectStateType, 
    PERComponentType inputComponentType, PERComponentType aiComponenentType, 
    PERComponentType physicsComponenentType, PERComponentType graphicsComponenentType, 
    InputData& inputData, AiData& aiData, PhysicsData& physicsData, GraphicsData& graphicsData)
{
    ObjectFactory* factory
        = new ObjectFactory(objectType,  objectStateType,
            inputComponentType, aiComponenentType, physicsComponenentType, graphicsComponenentType,
            inputData, aiData, physicsData, graphicsData
        );
    
    m_objectFactories.insert(std::pair<PERObjectType, ObjectFactory*>(objectType, factory));

    return factory;
}

void ObjectStorage::DeleteObjectFactories()
{
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        auto it = m_objectFactories.find((PERObjectType)type);
        if (it == m_objectFactories.end()) continue;

        ObjectFactory* objectFactory = it->second;
        delete objectFactory;
    }
}

void ObjectStorage::FillObjectQueues()
{
    
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {
        std::queue<PERObject*> queue;
        
        auto it = m_objectFactories.find((PERObjectType)type);
        if (it == m_objectFactories.end()) continue;

        ObjectFactory* objectFactory = it->second;
        for (int i = 0; i < PER_DEFAULT_OBJECT_POOL_SIZE; ++i) {    
            queue.push(objectFactory->CreateObject());
        }
        m_objectQueues.insert(std::pair<PERObjectType, std::queue<PERObject*>>((PERObjectType)type, queue));
    }
}

void ObjectStorage::RefillObjectQueue(std::queue<PERObject*>& queue, PERObjectType type)
{
    auto it = m_objectFactories.find(type);
    if (it == m_objectFactories.end()) return;

    ObjectFactory* objectFactory = it->second;
    for (int i = 0; i < PER_DEFAULT_OBJECT_POOL_SIZE; ++i) {
        queue.push(objectFactory->CreateObject());
    }
}

void ObjectStorage::ClearObjectQueues()
{
    for (int type = (int)PERObjectType::PLAYER + 1; type < (int)PERObjectType::NUM_OBJECT_TYPE; ++type) {

        auto it = m_objectQueues.find((PERObjectType)type);
        if (it == m_objectQueues.end()) continue;

        std::queue<PERObject*> queue = it->second;
        while (!queue.empty()) {
            PERObject* object = queue.front();
            queue.pop();
            delete object;
        }
    }
}
