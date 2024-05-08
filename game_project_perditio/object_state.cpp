#include "stdafx.h"
#include "object_state.h"
#include "per_object.h"
#include "ui_element.h"

ObjectState::ObjectState()
{
	Initialize();
}

ObjectState::~ObjectState()
{
}

void ObjectState::UseIgnoreDamageTime(double dTime)
{
	m_damageDelay -= dTime;
}

void ObjectState::Initialize()
{
	// 대미지 무시
	m_damageDelay = 0.0;
	// 자동 회복
	m_recoverDelay = 0.0;
	m_recoverTime = c_DEFAULT_TIME_FOR_RECOVER;
	m_bodyRecoverPercent = c_DEFAULT_BODY_RECOVER_PERCENT;
	m_mindRecoverPercent = c_DEFAULT_MIND_RECOVER_PERCENT;

	// 기본 정보
	m_isHasCollsionDamage = false;
	m_isImmortal = false;
	// 스폰 위치
	m_spawnPosition = PERVec3(0.0, 0.0, 0.0);
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

void ObjectState::SetOwner(PERObject* object)
{
	m_owner = object;
}

PERObject* ObjectState::GetOwner()
{
	return m_owner;
}

bool ObjectState::GiveDamage(PERObject& opponent, short physical, short mind)
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
		GetOwner()->SetLifeTime(-1.0);
		GetOwner()->GetGraphics().RemoveFloatingUi();

		// 부모가 있을 경우 총알이나 칼날이므로 부모에게 경험치를 줌
		if (opponent.GetParent()) {
			opponent.GetParent()->GetObjectState().GiveExp(m_stat.level);
		}
		else {
			opponent.GetObjectState().GiveExp(m_stat.level);
		}
	}

	return true;
}

bool ObjectState::UseMind(int mind)
{
	if (m_currentMind < mind) return false;
	m_currentMind -= mind;

	return true;
}

void ObjectState::RecoverPerTime(double dTime)
{
}

void ObjectState::GiveExp(int exp)
{
	m_exp += exp;
	if (m_exp >= m_stat.level * c_DEFAULT_LEVEL_EXP_GAP) {
		m_exp -= m_stat.level * c_DEFAULT_LEVEL_EXP_GAP;
		m_stat.level++;
	}
}
