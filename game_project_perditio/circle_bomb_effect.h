#pragma once
#include "particle_effect.h"

class CircleBombEffect : public ParticleEffect {
public:
	CircleBombEffect(PERVec3 pos, double delay, double lifeTime);
	~CircleBombEffect();

	virtual void Update(PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer);
};