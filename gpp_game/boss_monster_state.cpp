#include "boss_monster_state.h"
#include "event_dispatcher.h"

void BossMonsterState::Initialize()
{
	MonsterState::Initialize();

	m_isHasCollsionDamage = true;
}

void BossMonsterState::KillSelf(PERWorld& world, PERAudio& audio)
{
	// 재활용 시 보스가 아닌 몬스터가 이벤트를 보내지 않도록 설정
	if (!m_isLiving) MonsterState::KillSelf(world, audio);
	else {
		m_isLiving = false;

		EventDispatcher::Send(PEREvent::GO_TO_CREDITS, PERVec3(0.0, 0.0, 0.0));
		//MonsterState::KillSelf(world, audio);
	}
}
