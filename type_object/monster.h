#pragma once
#include "stdafx.h"
#include "breed.h"

class Monster {
public:
	Monster(Breed& breed) : m_health(breed.GetHealth()), m_breed(breed) {}
	
	int GetHealth() const { return m_health; }
	const char* GetAttack() { return m_breed.GetAttack(); }

	void SetHealth(int health) { m_health = health; }

private:
	int m_health;
	Breed& m_breed;
};