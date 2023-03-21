#include "stdafx.h"
#include "breed.h"
#include "monster.h"

Breed::Breed(int health, const char* attack)
    : m_health(health), m_attack(attack) 
{

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
