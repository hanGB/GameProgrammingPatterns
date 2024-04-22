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
    if (!m_bodyBar) return;

    PERVec3 objectPos = object.GetPosition();
    PERVec3 objectSize = object.GetSize();
    PERVec2 pos = PERVec2(objectPos.x, objectPos.y + objectSize.y * 1.5);

    m_bodyBar->SetPosition(pos);
    dynamic_cast<ProgressBar*>(m_bodyBar)->MatchWithData(m_nameId, m_stat.body, m_currentBody);

    if (!m_nameTag) return;

    pos = PERVec2(objectPos.x, objectPos.y - objectSize.y * 1.5);
    m_nameTag->SetPosition(pos);
    pos = PERVec2(objectPos.x - objectSize.x, objectPos.y - objectSize.y * 1.25);
    dynamic_cast<NameTag*>(m_nameTag)->MatchWithData(m_nameId, pos);
}

bool MonsterState::ShowFloatingUi(PERObject& object, PERHud* hud)
{
    m_bodyBar = hud->GetNewUiElementInWorld(PERUiElementType::PROGRESS_BAR);

    if (!m_bodyBar) return false;

    m_bodyBar->SetSize(PERVec2(1.0, 0.2));
    dynamic_cast<ProgressBar*>(m_bodyBar)->Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.2), PERColor(200, 200, 200), PERColor(255, 0, 0), 100, 100);

    m_nameTag = hud->GetNewUiElementInWorld(PERUiElementType::NAME_TAG);

    if (!m_nameTag) return false;

    m_nameTag->SetSize(PERVec2(1.0, 0.25));
    dynamic_cast<NameTag*>(m_nameTag)->
        Init(PERVec2(0.0, 0.0), PERVec2(1.0, 0.25), PERColor(200, 200, 200), m_nameId, 0.35, PERVec2(0.0, 0.0), PERColor(50, 50, 50));

    MatchFloatingUI(object);

    return true;
}

void MonsterState::HideFloatingUi()
{
    if (m_bodyBar) {
        m_bodyBar->SetLifeTime(-1.0);
        m_bodyBar = nullptr;
    }
    if (m_nameTag) {
        m_nameTag->SetLifeTime(-1.0);
        m_nameTag = nullptr;
    }
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
