#pragma once
#include "per_hud.h"

class TestFieldHud : public PERHud {
public:
	TestFieldHud();
	~TestFieldHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

private:
	ProgressBar* m_bodyBar, * m_mindBar;
};