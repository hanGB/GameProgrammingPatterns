#pragma once
#include "per_particle.h"

class PERParticlePool {
public:
	PERParticlePool();
	~PERParticlePool();

	void Create(PERShapeType type, PERVec3 pos, PERVec3 size, double mass, PERVec3 force, PERVec3 vel, PERColor color,
		double lifeTime = PER_MAXIMUM_LIFE_TIME, bool isColletedByPlayer = false, bool isBorderOn = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	void Update(double dTime);
	void Renderer(PERRenderer& renderer);

private:
	PERParticle m_pools[PER_DEFAULT_MAX_PARTICLES];
	PERParticle* m_head;
};