#include "stdafx.h"
#include "breed.h"
#include "monster.h"

Breed::Breed(Breed* parent, int health, const char* attack)
    : m_health(health), m_attack(attack), m_parent(parent)
{
}

Monster* Breed::NewMonster()
{
    return new Monster(*this);
}

int Breed::GetHealth() const
{
    // 오버라이딩
    if (m_health != 0 || m_parent == NULL) {
        return m_health;
    }

    // 상속
    return m_parent->GetHealth();
}

const char* Breed::GetAttack()
{
    // 오버라이딩
    if (m_attack != nullptr || m_parent == NULL) {
        return m_attack;
    }

    // 상속
    return m_parent->GetAttack();
}
