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

    return true;
}

bool MonsterState::UseMind(int mind)
{
    if (!ObjectState::UseMind(mind)) return false;
    return true;
}

void MonsterState::KillSelf(PERWorld& world)
{
    ObjectState::KillSelf(world);

    PERStat stat;
    // 레벨을 이펙트 타입으로 사용
    stat.level = ( short ) PERParticleEffectType::CIRCLE_BOMB;
    // 바디를 흡수 여부로 사용
    stat.body = ( short ) true;

    world.RequestAddObject(GetOwner(), PERObjectType::PARTICLE_EFFECTER, "PARTICLE_EFFECT_BASIC_MONSTER_DEATH_VISUAL", PERDatabaseType::EFFECT,
        stat, GetOwner()->GetPosition(), 1.0, PERVec3(0.0, 0.0, 0.0));
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
