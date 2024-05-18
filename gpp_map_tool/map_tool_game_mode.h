#pragma once
#include "game_mode.h"

class MapToolGameMode : public GameMode {
public:
	MapToolGameMode();
	~MapToolGameMode();

	virtual void Update();
	virtual void UpdateCamera(PERRenderer& renderer, double frameGap);

private:
	// 플레이어 팩토리 생성
	virtual void CreatePlayerFactory();
};