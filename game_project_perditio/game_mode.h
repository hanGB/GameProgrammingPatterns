#pragma once
class PERObject;
class PERHud;
class ObjectPool;

class GameMode {
public:
	GameMode();
	~GameMode();

	void Init();
	PERObject* GetPlayer();
	PERHud* GetHud();

private:
	PERObject* m_player;
	PERHud* m_hud;
};