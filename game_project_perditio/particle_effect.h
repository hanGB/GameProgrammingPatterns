#pragma once
#include "per_particle_pool.h"

class PERAudio;

class ParticleEffect {
public:
	ParticleEffect();
	~ParticleEffect();

	void SetParticle(PERShapeType type, PERVec3 size, double mass, PERColor color,
		double lifeTime = PER_MAXIMUM_LIFE_TIME, bool isBorderOn = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	virtual void Update(PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer);

	bool IsLifeTimeEnd(double dTime);

protected:
	PERVec3 m_position;

	// 소환 딜레이
	double	m_delay;
	double	m_lifeTime;

	double	m_time = 0.0;

	PERParticlePool m_pool;

	// 소환 파티클 관련
	PERShapeType	m_particleShapeType;
	PERVec3			m_particleSize;
	double			m_particleMass;
	PERColor		m_particleColor;
	double			m_particleLifeTime;
	bool			m_isParticleBorderOn;
	int				m_particleBorderWidth;
	PERColor		m_particleBorderColor;
};