#pragma once
#include "stdafx.h"

class Wizard {
public:
	Wizard(std::string name, int health, int wisdom, int agility) : m_name(name)
	{
		m_status.health = health;
		m_status.wisdom = wisdom;
		m_status.agility = agility;
	}
	~Wizard() { }


	int GetHealth() const {
		std::cout << "Health value " << m_status.health << '\n';
		return m_status.health;
	}
	int GetWisdom() const {
		std::cout << "Wisdom value " << m_status.wisdom<< '\n';
		return m_status.wisdom;
	}
	int GetAgility() const {
		std::cout << "Agility value " << m_status.agility << '\n';
		return m_status.agility;
	}

	void SetHeallth(int amount) {
		std::cout << m_name << ": ";
		std::cout << "Health = " << amount << '\n';
		m_status.health = amount;
	}
	void SetWisdom(int amount) {
		std::cout << m_name << ": ";
		std::cout << "Wisdom = " << amount << '\n';
		m_status.wisdom = amount;
	}
	void SetAgility(int amount) {
		std::cout << m_name << ": ";
		std::cout << "Agility = " << amount << '\n';
		m_status.agility = amount;
	}

private:
	std::string m_name;

	struct Status {
		int health;
		int wisdom;
		int agility;
	} m_status;
};