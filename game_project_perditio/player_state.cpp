#include "stdafx.h"
#include "player_state.h"
#include "event_dispatcher.h"

bool PlayerState::GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind)
{
    if (!ObjectState::GiveDamage(object, opponent, physical, mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_BD, PERVec3(m_currentBody, 0.0, 0.0));
    return true;
}

bool PlayerState::UseMind(PERObject& object, int mind)
{
    if (!ObjectState::UseMind(object, mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_MD, PERVec3(m_currentMind, 0.0, 0.0));
    return true;
}

void PlayerState::RecoverPerTime(PERObject& object, double dTime)
{
    if (m_currentBody == m_stat.body && m_currentMind == m_stat.mind) return;

    m_recoverDelay += dTime;

    if (m_recoverDelay < m_recoverTime) return;

    // 특정량 회복
    m_currentBody = std::clamp(m_currentBody + (int)(m_stat.body * m_bodyRecoverPercent), 0, (int)m_stat.body);
    m_currentMind = std::clamp(m_currentMind + (int)(m_stat.mind * m_mindRecoverPercent), 0, (int)m_stat.mind);
    EventDispatcher::Send(PEREvent::UPDATE_BD, PERVec3(m_currentBody, 0.0, 0.0));
    EventDispatcher::Send(PEREvent::UPDATE_MD, PERVec3(m_currentMind, 0.0, 0.0));

    m_recoverDelay = 0.0;
}

void PlayerState::GiveExp(PERObject& object, int exp)
{
    ObjectState::GiveExp(object, exp);
    PERLog::Logger().InfoWithFormat("플레이어가 경험치 %d를 획득", exp);
}

double PlayerState::GetShootCoolTime() const
{
    return m_shootCoolTime;
}

std::string& PlayerState::GetBulletVisualId()
{
    return m_bulletVisualId;
}

double PlayerState::GetSwingCoolTime() const
{
    return m_swingCoolTime;
}

std::string& PlayerState::GetBladeVisualId()
{
    return m_bladeVisualId;
}
