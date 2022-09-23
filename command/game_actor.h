#pragma once
#include "stdafx.h"

class GameActor {
public:
	GameActor(const char* name) : m_name(name) {};
	~GameActor() {};

	void PrintName() {
		std::cout << m_name;
	}

private:
	const char* m_name;
};

class Unit : public GameActor {
public:
	Unit(const char* name, int x, int y) : GameActor(name), m_x(x), m_y(y) {};
	~Unit() {};

	void MoveTo(int x, int y) {
		m_x = x;
		m_y = y;
		PrintLocation();
	}

	void PrintLocation() {
		PrintName();
		std::cout << ": " << m_x << ", " << m_y << std::endl;
	}

	int GetX() const {
		return m_x;
	}
	int GetY() const {
		return m_y;
	}

private:
	int m_x, m_y;
};