#include "stdafx.h"
#include "black_board.h"

PERVec3 BlackBoard::m_playerPosition = PERVec3(0.0, 0.0, 0.0);
NavigationData BlackBoard::m_navigationData = NavigationData();

PERVec3 BlackBoard::GetPlayerPos()
{
    return m_playerPosition;
}

NavigationData& BlackBoard::GetNavigationData()
{
    return m_navigationData;
}

void BlackBoard::SetPlayerPos(PERVec3 pos)
{
    m_playerPosition = pos;
}
