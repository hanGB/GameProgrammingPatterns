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

	// ������Ʈ ����
	PERObjectType m_type;
	PERStat m_stat;

	// ���� ��ġ
	PERVec3 m_position;

	// ���� Ÿ�̸�
	double m_time = 0.0;
	// ������ ������Ʈ
	PERObject* m_spawnedObject = nullptr;
};