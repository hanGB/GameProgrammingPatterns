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

	void SetSpawner(std::string objectId, PERObjectType type, PERSpawnType spawnType, int numSpawnObject = PER_DEFAULT_MAX_OBJECTS,
		double timeGap = 0.0, double distance = 0.0, double lifeTime = PER_MAXIMUM_LIFE_TIME);

	void InformSpawnedObjectIsDead();

private:
	void SpawnWithTimer(PERWorld& world, double dTime);
	void SpawnWithLiving(PERWorld& world, double dTime);
	void SpawnWithDistance(PERWorld& world, double dTime);

	void RequsetSpawnObjcet(PERWorld& world);

	// 오브젝트 설정
	std::string m_objectId;
	PERObjectType m_type;
	// 스폰할 오브젝트 수
	int m_numSpawnObject = PER_DEFAULT_MAX_OBJECTS;

	// 스폰 간격
	double m_timeGap = 5.0;
	// 스폰 타이머
	double m_time = 0.0;

	// 스폰 거리의 제곱
	double m_distance2 = 4.0;

	// 스폰 오브젝트 수명
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;

	// 스폰된 오브젝트
	bool m_isSpawnedObjectDead = true;

	std::function<void(SpawnerAiComponent&, PERWorld&, double)> m_SpawnFuc;
};