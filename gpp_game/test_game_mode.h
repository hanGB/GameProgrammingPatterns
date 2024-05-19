#pragma once
#include "game_mode.h"

class TestGameMode : public GameMode {
public:
	TestGameMode();
	~TestGameMode();

	virtual void Update();
	virtual void UpdateCamera(PERRenderer& renderer, double frameGap);

private:
	virtual void InitGameMode();

	// 플레이어 팩토리 생성
	virtual void CreatePlayerFactory();
};