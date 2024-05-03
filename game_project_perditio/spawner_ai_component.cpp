#include "stdafx.h"
#include "spawner_ai_component.h"
#include "per_world.h"
#include "per_object.h"
#include "per_database.h"
#include "black_board.h"

void SpawnerAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
    m_SpawnFuc(*this, object, world, dTime);
}

void SpawnerAiComponent::SetData(PERComponent::AiData data)
{
}

void SpawnerAiComponent::Initialize(PERComponent::AiData data)
{
    m_timeGap = 5.0;
    m_time = 0.0;
    m_distance2 = 4.0;
    m_lifeTime = PER_MAXIMUM_LIFE_TIME;
    m_spawnedObject = nullptr;
    m_isSettingSpawnedObject = true;
    SetData(data);
}

void SpawnerAiComponent::SetSpawner(std::string objectId, PERObjectType type, PERSpawnType spawnType, double timeGap, double distance, double lifeTime)
{
    m_objectId = objectId;
    m_type = type;
    m_timeGap = timeGap;
    m_distance2 = distance * distance;
    m_lifeTime = lifeTime;

    switch (spawnType)
    {
    case PERSpawnType::TIME:
        m_SpawnFuc = &SpawnerAiComponent::SpawnWithTimer;
        break;
    case PERSpawnType::LIVE:
        m_SpawnFuc = &SpawnerAiComponent::SpawnWithLiving;
        break;
    case PERSpawnType::Close:
        m_SpawnFuc = &SpawnerAiComponent::SpawnWithDistance;
        break;
    }
}

void SpawnerAiComponent::SetSpawnedObject(PERObject* object)
{
    m_spawnedObject = object;
    m_isSettingSpawnedObject = true;
}

void SpawnerAiComponent::SpawnWithTimer(PERObject& object, PERWorld& world, double dTime)
{
    m_time += dTime;

    if (m_time < m_timeGap) return;

    m_time = 0.0;

	RequsetSpawnObjcet(object, world);
}

void SpawnerAiComponent::SpawnWithLiving(PERObject& object, PERWorld& world, double dTime)
{
    if (!m_isSettingSpawnedObject) return;

    if (!m_spawnedObject) 
    {
        m_isSettingSpawnedObject = false;
       RequsetSpawnObjcet(object, world);
    }

    if (m_spawnedObject && m_spawnedObject->GetLifeTime() <= 0.0) 
    {
        m_isSettingSpawnedObject = false;
        RequsetSpawnObjcet(object, world);
    }
}

void SpawnerAiComponent::SpawnWithDistance(PERObject& object, PERWorld& world, double dTime)
{
    PERVec3 playerPos = BlackBoard::GetPlayerPos();
    PERVec3 pos = object.GetPosition();

    if (!m_isSettingSpawnedObject) return;

    if (m_spawnedObject) return;

    if (m_distance2 > DistanceSquareAandB(playerPos, pos))
    {
        m_isSettingSpawnedObject = false;
        RequsetSpawnObjcet(object, world);
    }
}

void SpawnerAiComponent::RequsetSpawnObjcet(PERObject& object, PERWorld& world)
{
    MonsterData* data = world.GetDatabase().GetMonsterData(m_objectId.c_str());
    PERVec3 pos = object.GetPosition();

    // 월드에 추가 요청
    world.RequestAddObject(
        &object, m_type, m_objectId.c_str(), false,
        data->stat, pos, m_lifeTime);
}
