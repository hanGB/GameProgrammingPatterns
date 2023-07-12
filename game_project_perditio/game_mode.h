#pragma once
class ObjectFactory;

class PERHud;
class GameState;
class PERObject;
class ObjectState;


class GameMode {
public:
	GameMode();
	~GameMode();

	void StartUse();
	void EndUse();

	PERHud& GetHud();
	GameState& GetGameState();
	PERObject& GetPlayer();
	ObjectState& GetPlayerState();

private:
	void CreatePlayerFactory();

	ObjectFactory* m_playerFactory;

	PERHud* m_hud;
	GameState* m_gameState;
	PERObject* m_player;
	ObjectState* m_playerState;
};