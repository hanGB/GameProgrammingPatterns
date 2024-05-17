#pragma once
#include "per_game.h"

class GPPGame : public PERGame {
public:
	GPPGame(HWND hWnd);
	~GPPGame();

	virtual void Recive(PEREvent event, PERVec3 data);
};