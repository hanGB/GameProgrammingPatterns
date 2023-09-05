#include "stdafx.h"
#include "black_board.h"

PERVec3 BlackBoard::m_playerPosition = PERVec3(0.0, 0.0, 0.0);

PERVec3 BlackBoard::GetPlayerPos()
{
    return m_playerPosition;
}

void BlackBoard::SetPlayerPos(PERVec3 pos)
{
    m_playerPosition = pos;
}
