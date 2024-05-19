#pragma once
#include "per_hud.h"

class RespawnHud : public PERHud {
public:
	RespawnHud();
	~RespawnHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

	void ChangeSelectedMenu();

private:
	SelectBox* m_menu[2];
	int m_numMenu = 2;
	int m_selectedMenu = 0;
};