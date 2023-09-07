#pragma once
#include "navigation_data.h"

class BlackBoard {
public:
	static PERVec3 GetPlayerPos();
	static NavigationData& GetNavigationData();

	static void SetPlayerPos(PERVec3 pos);

private:
	static NavigationData m_navigationData;

	static PERVec3 m_playerPosition;
};