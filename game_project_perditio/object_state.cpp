#include "stdafx.h"
#include "object_state.h"
#include "per_object.h"

ObjectState::ObjectState()
{
}

ObjectState::~ObjectState()
{
}

void ObjectState::UseIgnoreDamageTime(double dTime)
{
	m_damageDelay -= dTime;
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

PERVec3 ObjectState::GetSpawnPosition() const
{
	return m_spawnPosition;
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

void ObjectState::SetSpawnPosition(PERVec3 position)
{
	m_spawnPosition = position;
}

bool ObjectState::GiveDamage(PERObject& object, short physical, short mind)
{
	if (physical == 0 && mind == 0) return false;
	if (m_damageDelay > 0.0) return false;

	int physicalDamage = physical - m_stat.physicalDefense;
	int mindDamage = mind - m_stat.mindDefense;

	physicalDamage = std::clamp(physicalDamage, 0, 999);
	mindDamage = std::clamp(mindDamage, 0, 999);

	int finalDamage = physicalDamage + mindDamage;
	finalDamage = std::clamp(finalDamage, 1, 999);

	m_currentBody -= finalDamage;

	m_damageDelay = c_DEFAULT_IGNORE_DAMAGE_TIME;

	return true;
}

bool ObjectState::UseMind(PERObject& object, int mind)
{
	if (m_currentMind < mind) return false;
	m_currentMind -= mind;

	return true;
}

void ObjectState::RecoverPerTime(PERObject& object, double dTime)
{
}
