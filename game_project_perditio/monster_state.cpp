#include "stdafx.h"
#include "monster_state.h"
#include "per_object.h"
#include "progress_bar.h"

bool MonsterState::GiveDamage(PERObject& object, short physical, short mind)
{
    if (!ObjectState::GiveDamage(object, physical, mind)) return false;
    dynamic_cast<ProgressBar*>(object.GetFloatingUi())->SetCurrent(m_currentBody);
    dynamic_cast<ProgressBar*>(object.GetFloatingUi())->UpateShowingValueImmediately();
    return true;
}

bool MonsterState::UseMind(PERObject& object, int mind)
{
    if (!ObjectState::UseMind(object, mind)) return false;
    return true;
}
