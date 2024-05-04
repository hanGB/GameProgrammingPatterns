#include "stdafx.h"
#include "particle_effect.h"

ParticleEffect::ParticleEffect()
{
	SetParticle(PERShapeType::RECTANGLE, PERVec3(0.25, 0.25, 0.25), 10, PERColor(255, 0, 0));
}

ParticleEffect::~ParticleEffect()
{
}

void ParticleEffect::SetParticle(PERShapeType type, PERVec3 size, double mass, PERColor color, 
	double lifeTime, bool isBorderOn, int borderWidth, PERColor borderColor)
{
	m_particleShapeType = type;
	m_particleSize		= size;
	m_particleMass		= mass;
	m_particleColor		= color;
	m_particleLifeTime = lifeTime;
	m_isParticleBorderOn	= isBorderOn;
	m_particleBorderWidth	= borderWidth;
	m_particleBorderColor	= borderColor;
}

void ParticleEffect::Update(PERAudio& audio, double dTime)
{
	m_time += dTime;

	m_pool.Update(dTime);
}

void ParticleEffect::Render(PERRenderer& renderer)
{
	m_pool.Renderer(renderer);
}

bool ParticleEffect::IsLifeTimeEnd(double dTime)
{
	m_lifeTime -= dTime;
	return m_lifeTime < 0;
}
