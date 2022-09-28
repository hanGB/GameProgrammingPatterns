#pragma once
#include "stdafx.h"

class Monster {
public:
	virtual ~Monster() {};
	virtual void Render() = 0;
};

class Ghost : public Monster {
public:
	Ghost(int hp, int mp) : m_hp(hp), m_mp(mp) { }

	virtual void Render() override {
		std::cout << "Ghost -> " << "hp: " << m_hp << " / mp: " << m_hp << '\n';
	}

private:
	int m_hp;
	int m_mp;
};

class Demon : public Monster {
public:
	virtual void Render() override {
		std::cout << "Demon" << '\n';
	}
};

class Sorcerer : public Monster {
public:
	virtual void Render() override {
		std::cout << "Sorcerer" << '\n';
	}
};
