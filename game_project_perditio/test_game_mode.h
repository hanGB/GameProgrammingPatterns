#pragma once
#include "game_mode.h"
#include "game_state.h"

class TestGameMode : public GameMode {
public:
	TestGameMode(GameState* gameState, UiElementPool* uiElementPool);
	~TestGameMode();

	virtual void StartUse();
	virtual void EndUse();
	virtual void Update();
	virtual void UpdateCamera(PERRenderer& renderer, double frameGap);

private:
	// �÷��̾� ���丮 ����
	virtual void CreatePlayerFactory();
	// hud ����
	virtual PERHud* CreateHud(UiElementPool* uiElementPool);
};