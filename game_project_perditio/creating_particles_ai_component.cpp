#include "stdafx.h"
#include "creating_particles_ai_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_particle_pool.h"

short CreatingParticlesAiComponent::m_powerAbsorptionSpawnOrder[9] 
	= { (short)280, (short)40, (short)200, (short)120, (short)240, (short)80, (short)0, (short)160, (short)320 };

void CreatingParticlesAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	m_CreateParticles(*this, world, audio, dTime);

	AiComponent::Update(world, audio, dTime);
}

void CreatingParticlesAiComponent::SetData(PERComponent::AiData data)
{
	switch (data.particleEffectType)
	{
	case PERParticleEffectType::CIRCLE_BOMB:
		m_CreateParticles = &CreatingParticlesAiComponent::CreateParticlesForCircleBombEffect;
		break;
	case PERParticleEffectType::POWER_ABSORPTION:
		m_CreateParticles = &CreatingParticlesAiComponent::CreateParticlesForPowerAbsorptionEffect;
		break;
	}

	AiComponent::SetData(data);
}

void CreatingParticlesAiComponent::Initialize()
{
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
	m_order = 0;

	AiComponent::Initialize();
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
	if (m_time > m_particleDelay) 
	{
		PERVec3 pos = GetOwner()->GetPosition();

		for (int i = 0; i != 360; i += (int)((double)360 / (double)m_particleAmount)) {
			PERVec3 vel = PERVec3(std::cos(i * ONE_DEGREE_IN_RADIAN) * 5.0, std::sin(i * ONE_DEGREE_IN_RADIAN) * 5.0, 0.0);

			world.GetParticlePool().Create(m_particleShapeType, pos, m_particleSize, 10.0, PERVec3(0.0, 0.0, 0.0), vel * m_particleSpeedRate,
				m_particleColor, m_particleLifeTime, m_isCollectedByPlayer, m_isParticleBorderOn, m_particleBorderWidth, m_particleBorderColor);
		}
		m_time = 0.0;
		return;
	}

	m_time += dTime;
}

void CreatingParticlesAiComponent::CreateParticlesForPowerAbsorptionEffect(PERWorld& world, PERAudio& audio, double dTime)
{
	if (m_time > m_particleDelay && m_order < m_particleAmount)
	{
		PERVec3 pos = GetOwner()->GetPosition();

		PERVec3 finalPos = PERVec3(pos.x + std::cos((int)m_powerAbsorptionSpawnOrder[m_order] * ONE_DEGREE_IN_RADIAN) * 1.5,
		pos.y + std::sin((int)m_powerAbsorptionSpawnOrder[m_order] * ONE_DEGREE_IN_RADIAN) * 1.5, 0.1);

		world.GetParticlePool().Create(m_particleShapeType, finalPos, m_particleSize, 10.0, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0),
			m_particleColor, m_particleLifeTime, m_isCollectedByPlayer, m_isParticleBorderOn, m_particleBorderWidth, m_particleBorderColor);

		m_time = 0.0;
		m_order++;
		return;
	}

	m_time += dTime;
}
