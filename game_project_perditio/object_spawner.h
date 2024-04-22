#pragma once
#include "per_data.h"

class PERObject;
class ObjectStorage;
struct VisualData;

class ObjectSpawner {
public:
	ObjectSpawner();
	ObjectSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos);
	~ObjectSpawner();

	void SetSpawner(std::string objectId, PERObjectType type, PERStat stat, VisualData* visualData, PERVec3 pos);

	PERObject* Spawn(ObjectStorage& stroage);
	PERObject* SpawnWithTimer(ObjectStorage& stroage, double timeGap, double dTime);
	PERObject* SpawnWithLiving(ObjectStorage& stroage);

private:
	PERObject* GetObjectWithSetting(ObjectStorage& stroage);

	// ������Ʈ ����
	std::string m_objectId;
	PERObjectType m_type;
	PERStat m_stat;
	
	// ���־�
	VisualData m_visualData;

	// ���� ��ġ
	PERVec3 m_position;

	// ���� Ÿ�̸�
	double m_time = 0.0;
	// ������ ������Ʈ
	PERObject* m_spawnedObject = nullptr;
};