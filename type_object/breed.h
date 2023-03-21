#pragma once

class Monster;

class Breed {
public:
	Breed(int health, const char* attack);

	// �� ������ ���� ����
	Monster* NewMonster();

	int GetHealth() const;
	const char* GetAttack();

private:
	int m_health;
	const char* m_attack;
};