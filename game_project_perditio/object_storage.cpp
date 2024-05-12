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

    // 초기화 후 큐에 넣음
    object->Initialize();
    m_objectQueues.find(type)->second.push(object);
}

void ObjectStorage::CreateObjectFactories()
{
    ObjectFactory* factory;
    std::vector<PERComponentType> inputTypes;
    std::vector<PERComponentType> aiTypes;
    std::vector<PERComponentType> physicsTypes;
    std::vector<PERComponentType> graphicsTypes;
    InputData input; AiData ai; PhysicsData physics; GraphicsData graphics;

    // fixed block
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::UNINTELLIGENT };
    physicsTypes = { PERComponentType::FIXED };
    graphicsTypes = { PERComponentType::VISIBLE };

    factory = CreateObjectFactory(PERObjectType::FIXED_BLOCK, PERObjectStateType::NON,
       inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);
    factory->SetSize(PERVec3(1.0, 1.0, 1.0));  factory->SetMass(100'000);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // movable block
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.borderColor = PERColor(0, 255, 0);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::UNINTELLIGENT };
    physicsTypes = { PERComponentType::MOVABLE };
    graphicsTypes = { PERComponentType::VISIBLE, PERComponentType::NAME_TAG_GRAPHICS };

    factory = CreateObjectFactory(PERObjectType::MOVABLE_BLOCK, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);
    
    factory->SetSize(PERVec3(1.0, 1.0, 1.0));  factory->SetMass(50);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // monster
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.shape = PERShapeType::TRIANGLE; graphics.color = PERColor(255, 0, 0);
    graphics.borderColor = PERColor(127, 0, 0);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::MONSTER_AI };
    physicsTypes = { PERComponentType::MOVABLE };
    graphicsTypes = { PERComponentType::VISIBLE, PERComponentType::NAME_TAG_GRAPHICS, PERComponentType::BODY_BAR_GRAPHICS };

    factory = CreateObjectFactory(PERObjectType::MONSTER, PERObjectStateType::MONSTER,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.25, 0.25, 0.25)); factory->SetMass(30);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // bullet
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.friction = false;
    graphics.shape = PERShapeType::ELLIPSE; graphics.color = PERColor(100, 100, 100);
    graphics.borderColor = PERColor(50, 50, 50);
   
    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::UNINTELLIGENT };
    physicsTypes = { PERComponentType::MOVABLE };
    graphicsTypes = { PERComponentType::VISIBLE };

    factory = CreateObjectFactory(PERObjectType::BULLET, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.2, 0.2, 0.2));     factory->SetMass(5);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // blade
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics); 
    physics.friction = false; 
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(200, 200, 200);
    graphics.borderColor = PERColor(50, 50, 50);
  
    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::UNINTELLIGENT };
    physicsTypes = { PERComponentType::STUCK };
    graphicsTypes = { PERComponentType::VISIBLE };

    factory = CreateObjectFactory(PERObjectType::BLADE, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.5, 0.5, 0.5));    factory->SetMass(10);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // spawner
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.isOccupySpace = false;

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::SPAWNER_AI };
    physicsTypes = { PERComponentType::FIXED };
    graphicsTypes = { PERComponentType::HIDDEN };

    factory = CreateObjectFactory(PERObjectType::SPAWNER, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));    factory->SetMass(100);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // button
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    physics.isOccupySpace = false;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(100, 100, 100);

    inputTypes = { PERComponentType::BUTTON_INPUT };
    aiTypes = { PERComponentType::MAKING_SIGNAL };
    physicsTypes = { PERComponentType::FIXED };
    graphicsTypes = { PERComponentType::VISIBLE, PERComponentType::NAME_TAG_GRAPHICS, PERComponentType::KEY_INPUT_HELPER_GRAPHICS };

    factory = CreateObjectFactory(PERObjectType::BUTTON, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(0.4, 0.8, 0.5));    factory->SetMass(50);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // pressure
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    ai.isSwitch = true;
    physics.isOccupySpace = false;
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(150, 150, 150);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::MAKING_SIGNAL };
    physicsTypes = { PERComponentType::PRESSURE_PHYSICS };
    graphicsTypes = { PERComponentType::VISIBLE, PERComponentType::NAME_TAG_GRAPHICS };

    factory = CreateObjectFactory(PERObjectType::PRESSURE, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));    factory->SetMass(100);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // door
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);
    graphics.shape = PERShapeType::RECTANGLE; graphics.color = PERColor(100, 50, 50);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::RESPONSE_TO_SIGNAL };
    physicsTypes = { PERComponentType::FIXED };
    graphicsTypes = { PERComponentType::VISIBLE, PERComponentType::NAME_TAG_GRAPHICS };

    factory = CreateObjectFactory(PERObjectType::DOOR, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(2.0, 1.0, 1.0));    factory->SetMass(100);

    // particle effecter
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::CREATING_PARTICLE };
    physicsTypes = { PERComponentType::FIXED };
    graphicsTypes = { PERComponentType::HIDDEN };

    factory = CreateObjectFactory(PERObjectType::PARTICLE_EFFECTER, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);

    // Trigger
    ObjectFactory::InitComponentDatas(input, ai, physics, graphics);

    inputTypes = { PERComponentType::NO_INTERACT };
    aiTypes = { PERComponentType::MAKING_SIGNAL, PERComponentType::RESPONSE_TO_SIGNAL };
    physicsTypes = { PERComponentType::TRIGGER_PHYSICS };
    graphicsTypes = { PERComponentType::HIDDEN_DEBUG };

    factory = CreateObjectFactory(PERObjectType::TRIGGER, PERObjectStateType::NON,
        inputTypes, aiTypes, physicsTypes, graphicsTypes,
        input, ai, physics, graphics);

    factory->SetSize(PERVec3(1.0, 1.0, 1.0));

    ClearVectors(inputTypes, aiTypes, physicsTypes, graphicsTypes);
}

ObjectFactory* ObjectStorage::CreateObjectFactory(
    PERObjectType objectType, PERObjectStateType objectStateType, 
    std::vector<PERComponentType> inputComponentTypes, std::vector<PERComponentType> aiComponenentTypes,
    std::vector<PERComponentType> physicsComponenentTypes, std::vector<PERComponentType> graphicsComponenentTypes,
    InputData& inputData, AiData& aiData, PhysicsData& physicsData, GraphicsData& graphicsData)
{
    ObjectFactory* factory
        = new ObjectFactory(objectType,  objectStateType,
            inputComponentTypes, aiComponenentTypes, physicsComponenentTypes, graphicsComponenentTypes,
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

void ObjectStorage::ClearVectors(std::vector<PERComponentType>& input, std::vector<PERComponentType>& ai, 
    std::vector<PERComponentType>& physics, std::vector<PERComponentType>& graphics)
{
    input.clear();
    ai.clear();
    physics.clear();
    graphics.clear();
}
