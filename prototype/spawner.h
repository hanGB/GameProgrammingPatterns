#pragma once
#include "stdafx.h"
#include "monster.h"

typedef Monster* (*SpawnCallback)();

class Spawner {
public:
	Spawner(SpawnCallback spawn, float spawnDelay) : m_spawn(spawn), m_spawnDelay(spawnDelay) {}
	~Spawner() {};

	Monster* SpawnMonster(float elpasedTimeInSec) {
		m_deltaTime += elpasedTimeInSec;
		if (m_deltaTime > m_spawnDelay) {
			m_deltaTime = 0.0f;
			std::cout << "Spawn monster: ";
			return m_spawn();
		}
		return nullptr;
	}

private:
	SpawnCallback m_spawn;

	Monster* m_protoype = nullptr;

	float m_spawnDelay = 1.0f;
	float m_deltaTime = 0.0f;;
};