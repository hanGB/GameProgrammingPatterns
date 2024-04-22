#include "stdafx.h"
#include "monster_state.h"
#include "per_object.h"
#include "progress_bar.h"
#include "per_hud.h"

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

void MonsterState::MatchFloatingUI(PERObject& object)
{
    if (!m_floatingUi) return;

    PERVec3 objectPos = object.GetPosition();
    PERVec3 objectSize = object.GetSize();
    PERVec2 pos = PERVec2(objectPos.x, objectPos.y + objectSize.y * 1.5);

    m_floatingUi->SetPosition(pos);
    m_floatingUi->MatchWithData(m_nameId, m_stat.body, m_currentBody);
}

bool MonsterState::ShowFloatingUi(PERObject& object, PERHud* hud)
{
    m_floatingUi = hud->GetNewUiElementInWorld(PERUiElementType::PROGRESS_BAR);

    if (!m_floatingUi) return false;

    m_floatingUi->SetSize(PERVec2(1.0, 0.2));
    dynamic_cast<ProgressBar*>(m_floatingUi)->Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.2), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);

    MatchFloatingUI(object);

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
