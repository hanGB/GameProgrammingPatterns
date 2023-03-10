#pragma once
#include "object.h"
#include "stdafx.h"

class Snake : public Object {
public:
	
	void GetPrevPos(int* x, int* y) 
	{
		*x = m_prevX[0];
		*y = m_prevY[0];
	}
	void SetPrevPos(int x, int y)
	{
		m_prevX[0] = x;
		m_prevY[0] = y;
	}

	void SavePrevAndCurrentPos() 
	{
		int x, y;
		GetPos(&x, &y);
		m_prevX[1] = m_prevX[0];
		m_prevY[1] = m_prevX[0];
		m_prevX[0] = x;
		m_prevY[0] = y;
	}
	void SavePrevPrevPosInPrevPos()
	{
		m_prevX[0] = m_prevX[1];
		m_prevY[0] = m_prevX[1];
	}

private:
	int m_prevX[2], m_prevY[2];
};