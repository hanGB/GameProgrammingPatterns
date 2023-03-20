#pragma once
#include "stdafx.h"
#include "object.h"
#include "sky_launch.h"
#include "sound_player.h"
#include "particle_spawner.h"

class Hero : public Object {
public:
	Hero() 
	{
		m_power = new SkyLaunch();
		m_power->SetObject(this);
	}
	~Hero() { delete m_power; }

	virtual void Render() {
		std::cout << "Player: ";
		Object::Render();
		std::cout << '\n';
	}
	virtual void Update() {
		m_power->UsePower();
	}

private:
	SkyLaunch* m_power;
};