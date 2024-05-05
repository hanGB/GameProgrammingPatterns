#include "stdafx.h"
#include "creating_particles_ai_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_particle_pool.h"

void CreatingParticlesAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	CreateParticlesForCircleBombEffect(object, world, audio, dTime);
}

void CreatingParticlesAiComponent::SetData(PERComponent::AiData data)
{
	m_particleDelay = data.particleDelay;
	m_particleLifeTime = data.particleLifeTime;

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
	m_time = m_particleDelay;

	m_particleShapeType = PERShapeType::RECTANGLE;
	m_particleSize = PERVec3(0.25, 0.25, 0.25);
	m_particleMass = 10;
	m_particleColor = PERColor(200, 200, 200);
	m_isParticleBorderOn = true;
	m_particleBorderWidth = 1;
	m_particleBorderColor = PERColor(0, 0, 0);
}

void CreatingParticlesAiComponent::SetParticle(PERShapeType type, PERVec3 size, double mass, PERColor color,
	bool isBorderOn, int borderWidth, PERColor borderColor)
{
	m_particleShapeType = type;
	m_particleSize = size;
	m_particleMass = mass;
	m_particleColor = color;
	m_isParticleBorderOn = isBorderOn;
	m_particleBorderWidth = borderWidth;
	m_particleBorderColor = borderColor;
}

void CreatingParticlesAiComponent::CreateParticlesForCircleBombEffect(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	PERVec3 pos = object.GetPosition();

	if (m_time > m_particleDelay) {
		for (int i = 0; i != 360; i += 20) {
			PERVec3 vel = PERVec3(std::cos(i * ONE_DEGREE_IN_RADIAN) * 5.0, std::sin(i * ONE_DEGREE_IN_RADIAN) * 5.0, 0.1);

			world.GetParticlePool().Create(m_particleShapeType, pos, m_particleSize, m_particleMass, PERVec3(0.0, 0.0, 0.0), vel,
				m_particleColor, m_particleLifeTime, m_isParticleBorderOn, m_particleBorderWidth, m_particleBorderColor);
		}
		m_time = 0.0;
		return;
	}

	m_time += dTime;
}
