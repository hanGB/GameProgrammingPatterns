#include "stdafx.h"
#include "circle_bomb_effect.h"
#include "per_log.h"

CircleBombEffect::CircleBombEffect(PERVec3 pos, double delay, double lifeTime)
{
	m_position = pos;
	m_delay = delay;
	m_lifeTime = lifeTime;
	m_time = delay;
}

CircleBombEffect::~CircleBombEffect()
{

}

void CircleBombEffect::Update(PERAudio& audio, double dTime)
{
	if (m_time > m_delay) {
		for (int i = 0; i != 360; i += 20) {
			PERVec3 vel = PERVec3(std::cos(i * ONE_DEGREE_IN_RADIAN) * 5.0, std::sin(i * ONE_DEGREE_IN_RADIAN) * 5.0, 0.1);

			m_pool.Create(m_particleShapeType, m_position, m_particleSize, m_particleMass, PERVec3(0.0, 0.0, 0.0), vel,
				m_particleColor, m_particleLifeTime, m_isParticleBorderOn, m_particleBorderWidth, m_particleBorderColor);
		}
		m_time = 0.0;
	}

	ParticleEffect::Update(audio, dTime);
}

void CircleBombEffect::Render(PERRenderer& renderer)
{
	ParticleEffect::Render(renderer);
}
