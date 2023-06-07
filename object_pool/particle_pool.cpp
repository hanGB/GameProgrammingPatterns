#include "stdafx.h"
#include "particle_pool.h"

ParticlePool::ParticlePool()
{
	// ó�� ��ƼŬ���� ��� ����
	m_firstAvailable = &m_particles[0];

	// ��� ��ƼŬ�� ���� ��ƼŬ�� ����Ŵ
	for (int i = 0; i < POOL_SIZE - 1; ++i) {
		m_particles[i].SetNext(&m_particles[i + 1]);
	}

	// ������ ��ƼŬ���� ����Ʈ ����
	m_particles[POOL_SIZE - 1].SetNext(nullptr);
}

void ParticlePool::Craete(double x, double y, double xVel, double yVel, double lifeTime)
{
	// Ǯ�� ��� ���� ������ Ȯ��
	if (m_firstAvailable == nullptr) return;

	// ���� ��ƼŬ�� ��ĭ ��Ͽ��� ����
	Particle* newParticle = m_firstAvailable;
	m_firstAvailable = newParticle->GetNext();
	newParticle->Init(x, y, xVel, yVel, lifeTime);
}

void ParticlePool::Update(double time)
{
	for (auto& particle : m_particles) {
		if (particle.Update(time)) {
			// ��� ���� ��ƼŬ�� ��ĭ ����Ʈ �տ� �߰�
			particle.SetNext(m_firstAvailable);
			m_firstAvailable = &particle;
		}
	}
}

void ParticlePool::Render(HDC& memDC, CoordinateData& cd)
{
	for (auto& particle : m_particles) {
		particle.Render(memDC, cd);
	}
}
