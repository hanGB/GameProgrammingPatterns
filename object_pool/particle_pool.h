#pragma once
#include "particle.h"

class ParticlePool {
public:
	ParticlePool();

	void Craete(double x, double y, double xVel, double yVel, double lifeTime);
	void Update(double time);
	void Render(HDC& memDC, CoordinateData& cd);

private:
	static const int POOL_SIZE = 100;
	
	Particle m_particles[POOL_SIZE];
	Particle* m_firstAvailable;
};