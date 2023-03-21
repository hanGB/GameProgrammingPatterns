#pragma once

class Monster {
	friend class Breed;

public:
	int GetHealth() const;
	const char* GetAttack();

	void SetHealth(int health);

private:
	Monster(Breed& breed);

	int m_health;
	Breed& m_breed;
};