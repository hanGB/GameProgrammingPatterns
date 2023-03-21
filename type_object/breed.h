#pragma once

class Monster;

class Breed {
public:
	Breed(Breed* parent, int health, const char* attack);

	// 이 종족의 몬스터 생성
	Monster* NewMonster();

	int GetHealth() const;
	const char* GetAttack();

private:
	Breed* m_parent;
	int m_health = 0;
	const char* m_attack = nullptr;
};