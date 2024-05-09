#include "stdafx.h"
#include "player_state.h"
#include "event_dispatcher.h"
#include "per_world.h"
#include "per_object.h"

void PlayerState::Initialize()
{
    // 공격체 관련
    m_bulletVisualId = "PLAYER_BASIC_BULLET_VISUAL";
    m_shootCoolTime = c_DEFAULT_SHOOT_BULLET_COOL_TIME;
    m_bladeVisualId = "PLAYER_BASIC_BLADE_VISUAL";
    m_swingCoolTime = C_DEFAULT_SWING_BLADE_COOL_TIME;

    ObjectState::Initialize();
}

void PlayerState::Recive(PEREvent event, PERVec3 data)
{
    switch (event) {
    case PEREvent::ADD_BD_TO_PLAYER: {
        m_currentBody = std::clamp(m_currentBody + (int)data.x, 0, (int) m_stat.body);
        EventDispatcher::Send(PEREvent::UPDATE_BD, PERVec3(m_currentBody, 0.0, 0.0));
        break;
    }
    }
}

bool PlayerState::GiveDamage(PERObject& opponent, PERWorld& world, short physical, short mind)
{
    if (!ObjectState::GiveDamage(opponent, world, physical, mind)) return false;

    if ( m_currentBody <= 0 ) 
    {   
        PERStat stat;
        // 레벨을 이펙트 타입으로 사용
        stat.level = (short)PERParticleEffectType::CIRCLE_BOMB;
        // 바디를 흡수 여부로 사용
        stat.body = (short)false;

        world.RequestAddObject(GetOwner(), PERObjectType::PARTICLE_EFFECTER, "PARTICLE_EFFECT_PLAYPER_DEATH_VISUAL", PERDatabaseType::EFFECT, 
            stat, GetOwner()->GetPosition(), 1.0, PERVec3(0.0, 0.0, 0.0));
    }
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
    if (m_currentMind == m_stat.mind) return;

    m_recoverDelay += dTime;

    if (m_recoverDelay < m_recoverTime) return;

    // 특정량 회복
    m_currentMind = std::clamp(m_currentMind + (int)(m_stat.mind * m_mindRecoverPercent), 0, (int)m_stat.mind);
    EventDispatcher::Send(PEREvent::UPDATE_MD, PERVec3(m_currentMind, 0.0, 0.0));

    m_recoverDelay = 0.0;
}

void PlayerState::GiveExp(PERWorld& world, int exp)
{
    ObjectState::GiveExp(world, exp);
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
