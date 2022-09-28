#pragma once
#include "stdafx.h"
#include "monster.h"

class Spawner {
public:
	virtual ~Spawner() {};
	virtual Monster* SpawnMonster(float elpasedTimeInSec) = 0;
};

template<class T>
class SpawnerFor : public Spawner {
public:
	SpawnerFor(float spawnDelay) : m_spawnDelay(spawnDelay) {}
	~SpawnerFor() {};

	virtual  Monster* SpawnMonster(float elpasedTimeInSec) override {
		m_deltaTime += elpasedTimeInSec;
		if (m_deltaTime > m_spawnDelay) {
			m_deltaTime = 0.0f;
			std::cout << "Spawn monster: ";
			return new T();
		}
		return nullptr;
	}

private:
	float m_spawnDelay = 1.0f;
	float m_deltaTime = 0.0f;;
};