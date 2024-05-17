#pragma once
#include "per_hud.h"

class PauseHud : public PERHud {
public:
	PauseHud();
	~PauseHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

	void ChangeSelectedMenu();

private:
	SelectBox* m_menu[4];
	int m_numMenu = 4;
	int m_selectedMenu = 0;
};