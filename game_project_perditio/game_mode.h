#pragma once
class ObjectFactory;

class PERHud;
class GameState;
class PERObject;
class ObjectState;
class PERRenderer;

class GameMode {
public:
	GameMode();
	~GameMode();

	void StartUse();
	void EndUse();
	void Update();
	void UpdateCamera(PERRenderer& renderer, double frameGap);

	PERHud& GetHud();
	GameState& GetGameState();
	PERObject& GetPlayer();

private:
	void CreatePlayerFactory();

	ObjectFactory* m_playerFactory;

	PERHud* m_hud;
	GameState* m_gameState;
	PERObject* m_player;
};