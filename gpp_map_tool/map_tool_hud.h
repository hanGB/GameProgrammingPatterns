#pragma once
#include "per_hud.h"

class MapToolHud : public PERHud {
public:
	MapToolHud();
	~MapToolHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

private:
};