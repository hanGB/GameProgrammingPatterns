#pragma once
#include "per_hud.h"

class TitleHud : public PERHud {
public:
	TitleHud();
	~TitleHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

private:
	SelectBox* m_menu[3];
};