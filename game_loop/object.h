#pragma once
#include "stdafx.h"

class Object {
public:
	virtual void ProcessInputs(KeyInputs& inputs) {}
	virtual void Update() {}
	virtual void Render() {}

	void SetPos(int x, int y) {
		m_posX = x;
		m_posY = y;
	}
	void GetPos(int* x, int* y) {
		*x = m_posX;
		*y = m_posY;
	}

private:
	int m_posX;
	int m_posY;
};