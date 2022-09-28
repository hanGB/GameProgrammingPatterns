#pragma once
#include "stdafx.h"
#include "monster.h"

class Spawner {
public:
	Spawner(Monster* protoype, float spawnDelay) : m_protoype(protoype), m_spawnDelay(spawnDelay) {}
	~Spawner() {};

	Monster* SpawnMonster(float elpasedTimeInSec) {
		m_deltaTime += elpasedTimeInSec;
		if (m_deltaTime > m_spawnDelay) {
			m_deltaTime = 0.0f;
			std::cout << "Spawn monster: ";
			return m_protoype->Clone();
		}
		return nullptr;
	}

private:

	Monster* m_protoype = nullptr;

	float m_spawnDelay = 1.0f;
	float m_deltaTime = 0.0f;;
};