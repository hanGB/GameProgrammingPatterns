#include "stdafx.h"
#include "black_board.h"

PERVec3 BlackBoard::m_playerPosition = PERVec3(0.0, 0.0, 0.0);
bool BlackBoard::m_isShowingName = false;
bool BlackBoard::m_isPlayerLiving = false;

NavigationData BlackBoard::m_navigationData = NavigationData();

PERVec3 BlackBoard::GetPlayerPos()
{
    return m_playerPosition;
}

bool BlackBoard::GetShowingName()
{
    return m_isShowingName;
}

bool BlackBoard::GetIsPlayerLiving()
{
    return m_isPlayerLiving;
}

NavigationData& BlackBoard::GetNavigationData()
{
    return m_navigationData;
}

void BlackBoard::SetPlayerPos(PERVec3 pos)
{
    m_playerPosition = pos;
}

void BlackBoard::SetShowingName(bool show)
{
    m_isShowingName = show;
}

void BlackBoard::SetIsPlayerLiving(bool live)
{
    m_isPlayerLiving = live;
}
