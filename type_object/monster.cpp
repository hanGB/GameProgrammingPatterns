#include "stdafx.h"
#include "monster.h"
#include "breed.h"

int Monster::GetHealth() const 
{ 
	return m_health; 
}
const char* Monster::GetAttack() 
{ 
	return m_breed.GetAttack();
}

void Monster::SetHealth(int health) 
{ 
	m_health = health; 
}

Monster::Monster(Breed& breed) 
	: m_health(breed.GetHealth()), m_breed(breed) 
{

}