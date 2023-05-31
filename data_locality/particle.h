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

	void SetLocation(double x, double y)
	{
		m_x = x;
		m_y = y;
	}

private:
	double m_x = 1, m_y = 0.0;
	double m_speed = 0.0;
	double m_elapsedTime = 0.0;
};

class ParticleSystem {
public:
	ParticleSystem() : m_numParticles(0), m_numActive(0)
	{

	}

	void Update(double time)
	{
		for (int i = 0; i < m_numActive; ++i) {
			m_particles[i].Update(time);
		}
	
	}
	void AddParticle(double x, double y) {
		if (c_MAX_PARTICLE > m_numParticles) {
			m_particles[m_numParticles].SetLocation(x, y);
			m_numParticles++;
		}


		int start = m_numParticles % 4;
		int gap = m_numParticles % 3 + 2;
		for (int i = start; i < m_numParticles; i += gap) {
			ActiveParticle(i);
		}
		start++;
		gap--;
		for (int i = start; i < m_numParticles; i += gap) {
			DeactivateParticle(i);
		}
	}

	void ActiveParticle(int index) {
		if (index >= m_numActive) {
			Particle temp = m_particles[m_numActive];
			m_particles[m_numActive] = m_particles[index];
			m_particles[index] = temp;

			m_numActive++;
		}
	}
	void DeactivateParticle(int index) {
		if (index < m_numActive) {
			Particle temp = m_particles[m_numActive];
			m_particles[m_numActive] = m_particles[index];
			m_particles[index] = temp;

			m_numActive--;
		}
	}

private:
	static const int c_MAX_PARTICLE = 1'000'000;

	int m_numParticles;
	int m_numActive;
	Particle m_particles[c_MAX_PARTICLE];
};