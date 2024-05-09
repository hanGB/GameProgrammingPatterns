#pragma once
#include "navigation_data.h"

class BlackBoard {
public:
	// getter
	static PERVec3 GetPlayerPos();
	static bool GetShowingName();
	static bool GetIsPlayerLiving();
	static NavigationData& GetNavigationData();

	// setter
	static void SetPlayerPos(PERVec3 pos);
	static void SetShowingName(bool show);
	static void SetIsPlayerLiving(bool live);

private:
	static NavigationData m_navigationData;

	static PERVec3 m_playerPosition;
	static bool m_isShowingName;
	static bool m_isPlayerLiving;
};