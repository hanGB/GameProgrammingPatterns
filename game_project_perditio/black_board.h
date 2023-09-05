#pragma once

class BlackBoard {
public:
	static PERVec3 GetPlayerPos();
	static void SetPlayerPos(PERVec3 pos);

private:
	static PERVec3 m_playerPosition;
};