#pragma once
#include "entity.h"
#include "world.h"

class Skeleton : public Entity {
public:
	Skeleton() : m_patrollingLeft(false) {}

	virtual void Update(double elapsedInSec)
	{
		if (m_patrollingLeft) {
			SetX(GetX() - SPEED * elapsedInSec);
			if (GetX() <= -MAX_PATROL_POSITION) m_patrollingLeft = false;
		} 
		else 
		{
			SetX(GetX() + SPEED * elapsedInSec);
			if (GetX() >= MAX_PATROL_POSITION) m_patrollingLeft = true;
		}
	}

private:
	const double SPEED = 5.0;
	const double MAX_PATROL_POSITION = (double)WORLD_WIDTH - 2.0;

	bool m_patrollingLeft;
};

class Lightning : public Entity {
public:
	Lightning() {}

	virtual void Update(double elapsedInSec)
	{
		SetY(GetY() -SPEED * elapsedInSec);

		if (GetY() < -((double)WORLD_HEIGHT - 2.0)) {
			SetIsDead(true);
		}
	}

private:
	const double SPEED = 5.0;
};

class Statue : public Entity {
public:
	Statue(double delay) : m_frames(0), m_delay(delay) {}

	virtual void Update(double elapsedInSec)
	{
		std::cout << m_frames << std::endl;

		m_frames += elapsedInSec;
		if (m_frames >= m_delay) {
			ShootLightning();
			
			m_frames = 0.0;
		}
	}
	void SetWorld(World* world) 
	{
		m_world = world;
	}


private:
	void ShootLightning()
	{
		Lightning* lightning = new Lightning();
		lightning->SetX(GetX());
		lightning->SetY(GetY());
		lightning->SetModel('L');
		m_world->AddEntityAfterOneLoopEnd(lightning);
	}

	double m_frames;
	double m_delay;
	World* m_world;
};
