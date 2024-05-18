#pragma once
#include "per_hud.h"

class NullHud : public PERHud {
public:
	NullHud();
	~NullHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

private:
};