#pragma once
#include "per_hud.h"

class MainMenuHud : public PERHud {
public:
	MainMenuHud();
	~MainMenuHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

private:

};