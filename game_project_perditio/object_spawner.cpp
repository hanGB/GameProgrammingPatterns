#include "stdafx.h"
#include "object_spawner.h"
#include "object_pool.h"
#include "per_object.h"

ObjectSpawner::ObjectSpawner()
{
}

ObjectSpawner::ObjectSpawner(PERObjectType type, PERStat stat, PERVec3 pos)
    : m_type(type), m_stat(stat), m_position(pos)
{
}

ObjectSpawner::~ObjectSpawner()
{
}

void ObjectSpawner::SetSpawner(PERObjectType type, PERStat stat, PERVec3 pos)
{
    m_type = type;
    m_stat = stat;
    m_position = pos;
}

PERObject* ObjectSpawner::Spawn(ObjectPool& pool)
{
    return GetObjectWithSetting(pool);
}

PERObject* ObjectSpawner::SpawnWithTimer(ObjectPool& pool, double timeGap, double dTime)
{
    m_time += dTime;

    if (m_time < timeGap) return nullptr;

    m_time = 0.0;

    return GetObjectWithSetting(pool);
}

PERObject* ObjectSpawner::SpawnWithLiving(ObjectPool& pool)
{
    if (!m_spawnedObject) {
        return GetObjectWithSetting(pool);
    }

    if (m_spawnedObject && m_spawnedObject->GetLifeTime() <= 0.0) {
        return GetObjectWithSetting(pool);
    }

    return nullptr;
}

PERObject* ObjectSpawner::GetObjectWithSetting(ObjectPool& pool)
{
    PERObject* object = pool.PopObject(m_type);
    object->GetObjectState().SetStat(m_stat);

    object->SetPosition(m_position);
    object->SetCurrentPositionToSpawnPosition();

    m_spawnedObject = object;
    return object;
}
