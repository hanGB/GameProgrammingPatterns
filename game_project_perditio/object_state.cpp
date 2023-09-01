#include "stdafx.h"
#include "object_state.h"

ObjectState::ObjectState()
{
}

ObjectState::~ObjectState()
{
}

PERStat ObjectState::GetStat() const
{
	return m_stat;
}

int ObjectState::GetCollisionDamage() const
{
	if (!m_isHasCollsionDamage) return 0;

	return m_stat.physicalAttack / 2;
}

int ObjectState::GetCurrentBody() const
{
	return m_currentBody;
}

int ObjectState::GetCurrentMind() const
{
	return m_currentMind;
}

void ObjectState::SetStat(PERStat stat)
{
	m_stat = stat;
	m_currentBody = m_stat.body;
	m_currentMind = m_stat.mind;
}

void ObjectState::SetIsHasCollisionDamage(bool damage)
{
	m_isHasCollsionDamage = damage;
}

void ObjectState::SetIsImmortal(bool immortal)
{
	m_isImmortal = immortal;
}

void ObjectState::GiveDamage(PERObject& object, short physical, short mind)
{
	if (physical == 0 && mind == 0) return;

	int physicalDamage = physical - m_stat.physicalDefense;
	int mindDamage = mind - m_stat.mindDefense;

	physicalDamage = std::clamp(physicalDamage, 0, 999);
	mindDamage = std::clamp(mindDamage, 0, 999);

	int finalDamage = physicalDamage + mindDamage;
	finalDamage = std::clamp(finalDamage, 1, 999);

	m_currentBody -= finalDamage;
}

bool ObjectState::UseMind(PERObject& object, int mind)
{
	if (m_currentMind < mind) return false;
	m_currentMind -= mind;

	return true;
}
