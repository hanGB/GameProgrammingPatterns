#pragma once
#include "stdafx.h"

class Breed {
public:
	Breed(int health, const char* attack) : m_health(health), m_attack(attack) {}

	int GetHealth() const { return m_health; }
	const char* GetAttack() { return m_attack; }

private:
	int m_health;
	const char* m_attack;
};