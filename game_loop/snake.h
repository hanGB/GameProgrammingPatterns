#pragma once
#include "object.h"
#include "stdafx.h"

class Snake : public Object {
public:
	void SaveCurrentPos() {
		int x, y;
		GetPos(&x, &y);
		m_prevX = x;
		m_prevY = y;
	}
	void GetPrevPos(int* x, int* y) {
		*x = m_prevX;
		*y = m_prevY;
	}

private:
	int m_prevX, m_prevY;
};