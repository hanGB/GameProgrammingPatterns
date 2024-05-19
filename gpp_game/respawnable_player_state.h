#pragma once

#include "player_state.h"
#include "per_world.h"

class RespawnablePlayerState : public PlayerState {
public:
	// 초기화
	virtual void Initialize();

	virtual void RecoverPerTime(PERWorld& world, PERAudio& audio, double dTime);
	virtual void KillSelf(PERWorld& world, PERAudio& audio);

private:
	const double c_RESPAWN_DELAY = 2.0;

	double m_respawnTime = c_RESPAWN_DELAY;
	bool m_isHaveToRespawn = false;
};