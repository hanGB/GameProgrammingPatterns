#pragma once
#include "per_game.h"

class MapToolGame : public PERGame {
public:
	MapToolGame(HWND hWnd);
	~MapToolGame();

	virtual void Recive(PEREvent event, PERVec3 data);
};