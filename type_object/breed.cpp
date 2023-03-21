#include "stdafx.h"
#include "breed.h"
#include "monster.h"

Breed::Breed(Breed* parent, int health, const char* attack)
    : m_health(health), m_attack(attack)
{
    // 오버라이드하지 않는 속성만 상속받는다
    if (parent != nullptr) {
        if (health == 0) m_health = parent->GetHealth();
        if (attack == nullptr) m_attack = parent->GetAttack();
    }
}

Monster* Breed::NewMonster()
{
    return new Monster(*this);
}

int Breed::GetHealth() const
{
    return m_health;
}

const char* Breed::GetAttack()
{
    return m_attack;
}
