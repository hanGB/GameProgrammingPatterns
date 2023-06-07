#include "stdafx.h"
#include "particle_pool.h"

ParticlePool::ParticlePool()
{
	// 처음 파티클부터 사용 가능
	m_firstAvailable = &m_particles[0];

	// 모든 파티클은 다음 파티클을 가리킴
	for (int i = 0; i < POOL_SIZE - 1; ++i) {
		m_particles[i].SetNext(&m_particles[i + 1]);
	}

	// 마지막 파티클에서 리스트 종료
	m_particles[POOL_SIZE - 1].SetNext(nullptr);
}

void ParticlePool::Craete(double x, double y, double xVel, double yVel, double lifeTime)
{
	// 풀이 비어 있지 않은지 확인
	if (m_firstAvailable == nullptr) return;

	// 얻은 파티클을 빈칸 목록에서 제거
	Particle* newParticle = m_firstAvailable;
	m_firstAvailable = newParticle->GetNext();
	newParticle->Init(x, y, xVel, yVel, lifeTime);
}

void ParticlePool::Update(double time)
{
	for (auto& particle : m_particles) {
		if (particle.Update(time)) {
			// 방금 죽은 파티클을 빈칸 리스트 앞에 추가
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
