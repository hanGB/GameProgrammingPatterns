#include "stdafx.h"
#include "player_state.h"
#include "event_dispatcher.h"

void PlayerState::Initialize()
{
    // 공격체 관련
    m_bulletVisualId = "PLAYER_BASIC_BULLET_VISUAL";
    m_shootCoolTime = c_DEFAULT_SHOOT_BULLET_COOL_TIME;
    m_bladeVisualId = "PLAYER_BASIC_BLADE_VISUAL";
    m_swingCoolTime = C_DEFAULT_SWING_BLADE_COOL_TIME;

    ObjectState::Initialize();
}

bool PlayerState::GiveDamage(PERObject& opponent, short physical, short mind)
{
    if (!ObjectState::GiveDamage(opponent, physical, mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_BD, PERVec3(m_currentBody, 0.0, 0.0));
    return true;
}

bool PlayerState::UseMind(int mind)
{
    if (!ObjectState::UseMind(mind)) return false;
    EventDispatcher::Send(PEREvent::UPDATE_MD, PERVec3(m_currentMind, 0.0, 0.0));
    return true;
}

void PlayerState::RecoverPerTime(double dTime)
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

void PlayerState::GiveExp(int exp)
{
    ObjectState::GiveExp(exp);
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
