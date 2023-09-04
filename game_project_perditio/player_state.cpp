#include "stdafx.h"
#include "player_state.h"
#include "event_dispatcher.h"

bool PlayerState::GiveDamage(PERObject& object, short physical, short mind)
{
    if (!ObjectState::GiveDamage(object, physical, mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_BD, PERVec3(m_currentBody, 0.0, 0.0));
    return true;
}

bool PlayerState::UseMind(PERObject& object, int mind)
{
    if (!ObjectState::UseMind(object, mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_MD, PERVec3(m_currentMind, 0.0, 0.0));
    return true;
}
