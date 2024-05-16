#pragma once
#include "per_hud.h"

class TitleHud : public PERHud {
public:
	TitleHud();
	~TitleHud();

	virtual void Recive(PEREvent event, PERVec3 data);
	virtual void MatchWithPlayerState(PlayerState& state);

	void ChangeSelectedMenu();

private:
	SelectBox* m_menu[3];
	int m_selectedMenu = 0;
};