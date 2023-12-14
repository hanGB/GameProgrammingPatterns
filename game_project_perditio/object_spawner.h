#pragma once
#include "per_stat.h"

class PERObject;
class ObjectPool;

class ObjectSpawner {
public:
	ObjectSpawner();
	ObjectSpawner(PERObjectType type, PERStat stat, PERVec3 pos);
	~ObjectSpawner();

	void SetSpawner(PERObjectType type, PERStat stat, PERVec3 pos);

	PERObject* Spawn(ObjectPool& pool);
	PERObject* SpawnWithTimer(ObjectPool& pool, double timeGap, double dTime);
	PERObject* SpawnWithLiving(ObjectPool& pool);

private:
	PERObject* GetObjectWithSetting(ObjectPool& pool);

	// 오브젝트 설정
	PERObjectType m_type;
	PERStat m_stat;

	// 스폰 위치
	PERVec3 m_position;

	// 스폰 타이머
	double m_time = 0.0;
	// 스폰된 오브젝트
	PERObject* m_spawnedObject = nullptr;
};