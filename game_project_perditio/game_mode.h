#pragma once
class ObjectFactory;

class PERHud;
class GameState;
class PERObject;
class PlayerState;
class PERRenderer;

class GameMode {
public:
	GameMode();
	~GameMode();

	virtual void StartUse();
	virtual void EndUse();
	virtual void Update() = 0;
	virtual void UpdateCamera(PERRenderer& renderer, double frameGap) = 0;

	PERHud& GetHud();
	GameState& GetGameState();
	PERObject& GetPlayer();
	PlayerState& GetPlayerState();

	void SetGameState(GameState* gameState);
	void SetHud(PERHud* hud);
	void UpdatePlayerState(PlayerState* updatedState);

protected:
	void InitGameMode();

	ObjectFactory* m_playerFactory;
	PERObject* m_player;

private:
	// 플레이어 팩토리 생성
	virtual void CreatePlayerFactory() = 0;

	GameState* m_gameState;
	PERHud* m_hud;
};