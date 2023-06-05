#include "stdafx.h"
#include "particle_pool.h"

void ParticlePool::Craete(double x, double y, double xVel, double yVel, double lifeTime)
{
	// ��� ���� ��ƼŬ �˻�
	for (auto& particle : m_particles) {
		if (!particle.InUse()) {
			particle.Init(x, y, xVel, yVel, lifeTime);
			return;
		}
	}
}

void ParticlePool::Update(double time)
{
	for (auto& particle : m_particles) {
		particle.Update(time);
	}
}

void ParticlePool::Render(HDC& memDC, CoordinateData& cd)
{
	for (auto& particle : m_particles) {
		particle.Render(memDC, cd);
	}
}
