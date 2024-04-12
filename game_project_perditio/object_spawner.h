#pragma once
#include "per_data.h"

class PERObject;
class ObjectPool;
struct VisualData;

class ObjectSpawner {
public:
	ObjectSpawner();
	ObjectSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos);
	~ObjectSpawner();

	void SetSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos);

	PERObject* Spawn(ObjectPool& pool);
	PERObject* SpawnWithTimer(ObjectPool& pool, double timeGap, double dTime);
	PERObject* SpawnWithLiving(ObjectPool& pool);

private:
	PERObject* GetObjectWithSetting(ObjectPool& pool);

	// 오브젝트 설정
	std::string m_objectId;
	PERObjectType m_type;
	PERStat m_stat;
	
	// 비주얼
	VisualData m_visualData;

	// 스폰 위치
	PERVec3 m_position;

	// 스폰 타이머
	double m_time = 0.0;
	// 스폰된 오브젝트
	PERObject* m_spawnedObject = nullptr;
};