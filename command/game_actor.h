#pragma once
#include "stdafx.h"

class GameActor {
public:
	GameActor(const char* name) : m_name(name) {};
	~GameActor() {};

	void Jump() {
		std::cout << m_name << ": Jumping\n";
	};

	void Fire() {
		std::cout << m_name << ": Fireing\n";
	};

	void Reload() {
		std::cout << m_name << ": Reloading\n";
	};

private:
	const char* m_name;
};