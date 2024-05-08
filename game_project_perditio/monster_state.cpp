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

bool MonsterState::GiveDamage(PERObject& opponent, PERWorld& world, short physical, short mind)
{
    if (!ObjectState::GiveDamage(opponent, world, physical, mind)) return false;

    if ( m_currentBody <= 0 )
    {
        PERStat stat;
        // 레벨을 이펙트 타입으로 사용
        stat.level = 0;
        // 바디를 흡수 여부로 사용
        stat.body = 1;
        // accel의 x을 파티클 딜레이로 y을 파티클 수명으로
        PERVec3 accel = PERVec3(5.0, 2.0, 1.5);
        // 사망 이펙트 추가 요청
        world.RequestAddObject(GetOwner(), PERObjectType::PARTICLE_EFFECTER, "PARTICLE_EFFECT_DEATH_VISUAL", true, stat, GetOwner()->GetPosition(), 1.0, accel);
    }

    return true;
}

bool MonsterState::UseMind(int mind)
{
    if (!ObjectState::UseMind(mind)) return false;
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
