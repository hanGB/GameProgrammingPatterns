#include "stdafx.h"
#include "object_spawner.h"
#include "object_pool.h"
#include "per_object.h"

ObjectSpawner::ObjectSpawner()
{
}

ObjectSpawner::ObjectSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos)
    : m_objectId(objectId), m_type(type), m_stat(stat), m_visualData(*visualData), m_position(pos)
{
}

ObjectSpawner::~ObjectSpawner()
{
}

void ObjectSpawner::SetSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos)
{
    m_objectId = objectId;
    m_type = type;
    m_stat = stat;
    m_visualData = *visualData;
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

    // ����
    object->GetObjectState().SetStat(m_stat);
    // ��ġ
    object->SetPosition(m_position);
    object->SetCurrentPositionToSpawnPosition();
    // ũ��
    object->SetSize(m_visualData.size);
    object->SetMass(m_visualData.mass);
    object->SetBoundingType(m_visualData.boundingType);
    // �׷���
    PERComponent::GraphicsData graphicsData;
    graphicsData.shape = m_visualData.shape; graphicsData.color = m_visualData.color;
    graphicsData.border = m_visualData.borderOn; graphicsData.borderWidth = m_visualData.borderWidth; graphicsData.borderColor = m_visualData.borderColor;
    graphicsData.floatingUi = m_visualData.floatingUiOn; graphicsData.distanceVisiblefloatingUi = m_visualData.floatingUiVisibleDistance;
    object->GetGraphics().SetData(graphicsData);

    m_spawnedObject = object;
    return object;
}