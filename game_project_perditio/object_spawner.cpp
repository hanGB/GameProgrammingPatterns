#include "stdafx.h"
#include "object_spawner.h"
#include "object_storage.h"
#include "per_object.h"

ObjectSpawner::ObjectSpawner()
{
}

ObjectSpawner::ObjectSpawner(std::string objectId, PERObjectType type, std::string nameId, PERStat stat, VisualData* visualData, PERVec3 pos)
    : m_objectId(objectId), m_type(type), m_nameId(nameId), m_stat(stat), m_visualData(*visualData), m_position(pos)
{
}

ObjectSpawner::~ObjectSpawner()
{
}

void ObjectSpawner::SetSpawner(std::string objectId, PERObjectType type, std::string nameId, PERStat stat, VisualData* visualData, PERVec3 pos)
{
    m_objectId = objectId;
    m_type = type;
    m_nameId = nameId;
    m_stat = stat;
    m_visualData = *visualData;
    m_position = pos;
}

PERObject* ObjectSpawner::Spawn(ObjectStorage& stroage)
{
    return GetObjectWithSetting(stroage);
}

PERObject* ObjectSpawner::SpawnWithTimer(ObjectStorage& stroage, double timeGap, double dTime)
{
    m_time += dTime;

    if (m_time < timeGap) return nullptr;

    m_time = 0.0;

    return GetObjectWithSetting(stroage);
}

PERObject* ObjectSpawner::SpawnWithLiving(ObjectStorage& stroage)
{
    if (!m_spawnedObject) {
        return GetObjectWithSetting(stroage);
    }

    if (m_spawnedObject && m_spawnedObject->GetLifeTime() <= 0.0) {
        return GetObjectWithSetting(stroage);
    }

    return nullptr;
}

PERObject* ObjectSpawner::GetObjectWithSetting(ObjectStorage& stroage)
{
    PERObject* object = stroage.PopObject(m_type);

    // 이름
    object->GetObjectState().SetNameId(m_nameId);
    // 스탯
    object->GetObjectState().SetStat(m_stat);
    // 위치
    object->SetPosition(m_position);
    object->SetCurrentPositionToSpawnPosition();
    // 크기
    object->SetSize(m_visualData.size);
    object->SetMass(m_visualData.mass);
    object->SetBoundingType(m_visualData.boundingType);
    // 그래픽
    PERComponent::GraphicsData graphicsData;
    graphicsData.shape = m_visualData.shape; graphicsData.color = m_visualData.color;
    graphicsData.border = m_visualData.borderOn; graphicsData.borderWidth = m_visualData.borderWidth; graphicsData.borderColor = m_visualData.borderColor;
    graphicsData.floatingUi = m_visualData.floatingUiOn; graphicsData.distanceVisiblefloatingUi = m_visualData.floatingUiVisibleDistance;
    object->GetGraphics().SetData(graphicsData);

    m_spawnedObject = object;
    return object;
}