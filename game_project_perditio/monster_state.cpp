#include "stdafx.h"
#include "monster_state.h"
#include "per_object.h"
#include "progress_bar.h"
#include "per_hud.h"

void MonsterState::Initialize()
{
    // 시야의 제곱
    m_sight = c_DEFAULT_SIGHT;
    // 배회 거리의 제곱
    m_wanderDistance = c_DEFAULT_WANDER_DISTANCE;
    ObjectState::Initialize();
}

bool MonsterState::GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind)
{
    if (!ObjectState::GiveDamage(object, opponent, physical, mind)) return false;

    return true;
}

bool MonsterState::UseMind(PERObject& object, int mind)
{
    if (!ObjectState::UseMind(object, mind)) return false;
    return true;
}

void MonsterState::SetSight(double sight)
{
    m_sight = sight;
}

void MonsterState::SetWanderDistance(double distance)
{
    m_wanderDistance = distance;
}

double MonsterState::GetSight() const
{
    return m_sight;
}

double MonsterState::GetSightSquare() const
{
    return m_sight * m_sight;
}

double MonsterState::GetWanderDistance() const
{
    return m_wanderDistance;
}

double MonsterState::GetWanderDistanceSquare() const
{
    return m_wanderDistance * m_wanderDistance;
}
