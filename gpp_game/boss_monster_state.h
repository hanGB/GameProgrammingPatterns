#pragma once

#include "monster_state.h"

class BossMonsterState : public MonsterState {
public:
	// 초기화
	virtual void Initialize();

	virtual void KillSelf(PERWorld& world, PERAudio& audio);

private:
	bool m_isLiving = true;
};