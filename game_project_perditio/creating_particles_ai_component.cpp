#include "stdafx.h"
#include "creating_particles_ai_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_particle_pool.h"

void CreatingParticlesAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	CreateParticlesForCircleBombEffect(world, audio, dTime);
}

void CreatingParticlesAiComponent::SetData(PERComponent::AiData data)
{
	switch (data.particleEffectType)
	{
	case PERParticleEffectType::CIRCLE_BOMB:
		m_CreateParticles = &CreatingParticlesAiComponent::CreateParticlesForCircleBombEffect;
		break;
	}
}

void CreatingParticlesAiComponent::Initialize(PERComponent::AiData data)
{
	SetData(data);

	m_particleShapeType = PERShapeType::RECTANGLE;
	m_particleSize = PERVec3(0.25, 0.25, 0.25);
	m_particleAmount = 36;
	m_particleSpeedRate = 1.0;
	m_particleDelay = 1.0;
	m_particleLifeTime = 3.0;
	m_particleColor = PERColor(200, 200, 200);
	m_isParticleBorderOn = true;
	m_particleBorderWidth = 1;
	m_particleBorderColor = PERColor(0, 0, 0);

	m_isCollectedByPlayer = false;
	m_time = m_particleDelay;
}

void CreatingParticlesAiComponent::SetParticle(PERShapeType type, PERVec3 size, int amount, 
	double speedRate, double spawnDelay, double particleLifeTime, bool isCollectedByPlayer, 
	PERColor color, bool isBorderOn, int borderWidth, PERColor borderColor)
{
	m_particleShapeType = type;
	m_particleSize = size;
	m_particleAmount = amount;
	m_particleSpeedRate = speedRate;
	m_particleDelay = spawnDelay;
	m_particleLifeTime = particleLifeTime;
	m_particleColor = color;
	m_isParticleBorderOn = isBorderOn;
	m_particleBorderWidth = borderWidth;
	m_particleBorderColor = borderColor;

	m_isCollectedByPlayer = isCollectedByPlayer;
	m_time = m_particleDelay;
}

void CreatingParticlesAiComponent::CreateParticlesForCircleBombEffect(PERWorld& world, PERAudio& audio, double dTime)
{
	PERVec3 pos = GetOwner()->GetPosition();

	if (m_time > m_particleDelay) {

		for (int i = 0; i != 360; i += (int)((double)360 / (double)m_particleAmount)) {
			PERVec3 vel = PERVec3(std::cos(i * ONE_DEGREE_IN_RADIAN) * 5.0, std::sin(i * ONE_DEGREE_IN_RADIAN) * 5.0, 0.1);

			world.GetParticlePool().Create(m_particleShapeType, pos, m_particleSize, 10.0, PERVec3(0.0, 0.0, 0.0), vel * m_particleSpeedRate,
				m_particleColor, m_particleLifeTime, m_isCollectedByPlayer, m_isParticleBorderOn, m_particleBorderWidth, m_particleBorderColor);
		}
		m_time = 0.0;
		return;
	}

	m_time += dTime;
}
