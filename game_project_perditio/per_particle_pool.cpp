#include "stdafx.h"
#include "per_particle_pool.h"

PERParticlePool::PERParticlePool()
{
	m_head = &m_pools[0];

	for (int i = 0; i < PER_DEFAULT_MAX_PARTICLES - 1; ++i) {
		m_pools[i].SetNext(&m_pools[i + 1]);
	}
	m_pools[PER_DEFAULT_MAX_PARTICLES - 1].SetNext(nullptr);
}

PERParticlePool::~PERParticlePool()
{
}

void PERParticlePool::Create(PERShapeType type, PERVec3 pos, PERVec3 size, double mass, PERVec3 force, PERVec3 vel, PERColor color, 
	double lifeTime, bool isColletedByPlayer, bool isBorderOn, int borderWidth, PERColor borderColor)
{
	if (!m_head) return;

	PERParticle* particle = m_head;
	m_head = particle->GetNext();

	particle->Init(type, pos, size, mass, force, vel, color, lifeTime, isColletedByPlayer, isBorderOn, borderWidth, borderColor);
}

void PERParticlePool::Update(double dTime)
{
	for (auto& particle : m_pools) {
		if (!particle.Update(dTime)) {
			particle.SetIsInUse(false);
			particle.SetNext(m_head);
			m_head = &particle;
		}
	}
}

void PERParticlePool::Renderer(PERRenderer& renderer)
{
	for (auto& particle : m_pools) {
		particle.Render(renderer);
	}
}
