#pragma once
#include "stdafx.h"

class Particle {
public:
	void Update(double time)
	{
		m_speed = m_speed + (-9.8) * time;
		m_y = m_y + m_speed * time + 0.5 * (-9.8) * time * time;
		m_elapsedTime += time;
	}

	bool GetIsActive() const
	{
		return m_isActive;
	}

	void SetIsActive(bool active) {
		m_isActive = active;
	}
	void SetLocation(double x, double y)
	{
		m_x = x;
		m_y = y;
	}

private:
	bool m_isActive = false;

	double m_x = 1, m_y = 0.0;
	double m_speed = 0.0;
	double m_elapsedTime = 0.0;
};

class ParticleSystem {
public:
	ParticleSystem() : m_numParticles(0) 
	{

	}

	void Update(double time)
	{
		for (int i = 0; i < m_numParticles; ++i) {
			
			if (m_particles[i].GetIsActive()) {
				//std::cout << i << "�� - Ȱ��ȭ\n";
				m_particles[i].Update(time);
			}
		}
	
	}
	void AddParticle(double x, double y) {
		if (c_MAX_PARTICLE > m_numParticles) {
			m_particles[m_numParticles].SetIsActive(false);
			m_particles[m_numParticles].SetLocation(x, y);
			m_numParticles++;
		}


		int start = m_numParticles % 4;
		int gap = m_numParticles % 3 + 2;
		for (int i = start; i < m_numParticles; i += gap) {
			m_particles[i].SetIsActive(true);
		}
		start++;
		gap--;
		for (int i = start; i < m_numParticles; i += gap) {
			m_particles[i].SetIsActive(false);
		}
	}

private:
	static const int c_MAX_PARTICLE = 1'000'000;

	int m_numParticles;
	Particle m_particles[c_MAX_PARTICLE];
};