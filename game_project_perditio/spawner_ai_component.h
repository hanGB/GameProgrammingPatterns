#pragma once
#include "ai_component.h"

enum class PERSpawnType {
	TIME,
	LIVE,
	Close,
	NUM_SPAWN_TYPE
};

class SpawnerAiComponent : public AiComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	void SetSpawner(std::string objectId, PERObjectType type, PERSpawnType spawnType, 
		double timeGap = 0.0, double distance = 0.0, double lifeTime = PER_MAXIMUM_LIFE_TIME);

	void SetSpawnedObject(PERObject* spawnedObject);

private:
	void SpawnWithTimer(PERWorld& world, double dTime);
	void SpawnWithLiving(PERWorld& world, double dTime);
	void SpawnWithDistance(PERWorld& world, double dTime);

	void RequsetSpawnObjcet(PERWorld& world);

	// ������Ʈ ����
	std::string m_objectId;
	PERObjectType m_type;

	// ���� ����
	double m_timeGap = 5.0;
	// ���� Ÿ�̸�
	double m_time = 0.0;

	// ���� �Ÿ��� ����
	double m_distance2 = 4.0;

	// ���� ������Ʈ ����
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;

	// ������ ������Ʈ
	PERObject* m_spawnedObject = nullptr;
	bool m_isSettingSpawnedObject = true;

	std::function<void(SpawnerAiComponent&, PERWorld&, double)> m_SpawnFuc;
};