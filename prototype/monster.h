#pragma once
#include "stdafx.h"

class Monster {
public:
	virtual ~Monster() {};
	virtual void Render() = 0;
	virtual Monster* Clone() = 0;
};

class Ghost : public Monster {
public:
	virtual void Render() override {
		std::cout << "Ghost";
	}
	virtual Monster* Clone() override {
		return new Ghost();
	}
};

class Demon : public Monster {
public:
	virtual void Render() override {
		std::cout << "Demon";
	}
	virtual Monster* Clone() override {
		return new Demon();
	}
};

class Sorcerer : public Monster {
public:
	virtual void Render() override {
		std::cout << "Sorcerer";
	}
	virtual Monster* Clone() override {
		return new Sorcerer();
	}
};
