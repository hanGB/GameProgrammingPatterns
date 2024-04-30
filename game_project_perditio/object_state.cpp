#include "stdafx.h"
#include "object_state.h"
#include "per_object.h"
#include "ui_element.h"

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

std::string ObjectState::GetNameId() const
{
	return m_nameId;
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

void ObjectState::SetNameId(std::string& nameId)
{
	m_nameId = nameId;
}

void ObjectState::SetNameId(const char* name)
{
	m_nameId = name;
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

bool ObjectState::GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind)
{
	if (m_isImmortal) return false;
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

	if (m_currentBody <= 0) {
		object.SetLifeTime(-1.0);
		object.GetGraphics().RemoveFloatingUi();

		// 부모가 있을 경우 총알이나 칼날이므로 부모에게 경험치를 줌
		if (opponent.GetParent()) {
			opponent.GetParent()->GetObjectState().GiveExp(*opponent.GetParent(), m_stat.level);
		}
		else {
			opponent.GetObjectState().GiveExp(opponent, m_stat.level);
		}
	}

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

void ObjectState::GiveExp(PERObject& object, int exp)
{
	m_exp += exp;
	if (m_exp >= m_stat.level * c_DEFAULT_LEVEL_EXP_GAP) {
		m_exp -= m_stat.level * c_DEFAULT_LEVEL_EXP_GAP;
		m_stat.level++;
	}
}
