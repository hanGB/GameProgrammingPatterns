#include "stdafx.h"
#include "spawner_ai_component.h"
#include "per_world.h"
#include "per_object.h"
#include "per_database.h"
#include "black_board.h"

void SpawnerAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
    // 스폰 오브젝트 수가 0이 되면 더이상 스폰 금지
    if (m_numSpawnObject <= 0) return;

    m_SpawnFuc(*this, world, dTime);

    AiComponent::Update(world, audio, dTime);
}

void SpawnerAiComponent::SetData(PERComponent::AiData data)
{
    AiComponent::SetData(data);
}

void SpawnerAiComponent::Initialize()
{
    m_timeGap = 5.0;
    m_time = 0.0;
    m_distance2 = 4.0;
    m_lifeTime = PER_MAXIMUM_LIFE_TIME;
    m_numSpawnObject = PER_DEFAULT_MAX_OBJECTS;
    m_isSpawnedObjectDead = true;

    AiComponent::Initialize();
}

void SpawnerAiComponent::SetSpawner(
std::string objectId, PERObjectType type, PERSpawnType spawnType, int numSpawnObject, 
double timeGap, double distance, double lifeTime
)
{
    m_objectId = objectId;
    m_type = type;
    m_numSpawnObject = numSpawnObject;
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

void SpawnerAiComponent::InformSpawnedObjectIsDead()
{
    PERLog::Logger().Info("소환한 오브젝트가 죽음");
    m_isSpawnedObjectDead = true;
}

void SpawnerAiComponent::SpawnWithTimer(PERWorld& world, double dTime)
{
    m_time += dTime;

    if (m_time < m_timeGap) return;

    m_time = 0.0;
    m_numSpawnObject--;

	RequsetSpawnObjcet(world);
}

void SpawnerAiComponent::SpawnWithLiving(PERWorld& world, double dTime)
{
    if (!m_isSpawnedObjectDead) return;

    m_isSpawnedObjectDead = false;
    m_numSpawnObject--;
    RequsetSpawnObjcet(world);
}

void SpawnerAiComponent::SpawnWithDistance(PERWorld& world, double dTime)
{
    PERVec3 playerPos = BlackBoard::GetPlayerPos();
    PERVec3 pos = GetOwner()->GetPosition();

    if (!m_isSpawnedObjectDead) return;

    if (m_distance2 > DistanceSquareAandB(playerPos, pos))
    {
        m_isSpawnedObjectDead = false;
        m_numSpawnObject--;
        RequsetSpawnObjcet(world);
    }
}

void SpawnerAiComponent::RequsetSpawnObjcet(PERWorld& world)
{
    MonsterData* data = world.GetDatabase().GetMonsterData(m_objectId.c_str());
    PERVec3 pos = GetOwner()->GetPosition();

    // 월드에 추가 요청
    world.RequestAddObject(
        GetOwner(), m_type, m_objectId.c_str(), PERDatabaseType::MONSTER,
        data->stat, pos, m_lifeTime);
}
