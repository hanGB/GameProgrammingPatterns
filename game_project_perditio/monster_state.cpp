#include "stdafx.h"
#include "monster_state.h"
#include "per_object.h"
#include "progress_bar.h"

void MonsterState::MatchFloatingUI(UiElement* m_flotingUi)
{
    m_flotingUi->MatchWithData(m_nameId, m_stat.body, m_currentBody);
}

bool MonsterState::GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind)
{
    if (!ObjectState::GiveDamage(object, opponent, physical, mind)) return false;

    dynamic_cast<ProgressBar*>(object.GetFloatingUi())->SetCurrent(m_currentBody);
    dynamic_cast<ProgressBar*>(object.GetFloatingUi())->UpateShowingValueImmediately();
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
