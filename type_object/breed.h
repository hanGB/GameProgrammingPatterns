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
	int m_health;
	const char* m_attack;
};