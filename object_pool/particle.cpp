#include "stdafx.h"
#include "particle.h"

Particle::Particle() 
	: m_lifeTimeLeft(0.0)
{
}

Particle::~Particle()
{
}

void Particle::Init(double x, double y, double xVel, double yVel, double lifeTime)
{
	m_state.live.x = x;
	m_state.live.y = y;
	m_state.live.xVel = xVel;
	m_state.live.yVel = yVel;
	m_state.live.xSize = DEFUALT_PARTICLE_SIZE;
	m_state.live.ySize = DEFUALT_PARTICLE_SIZE;

	m_lifeTimeLeft = lifeTime;
}

bool Particle::Update(double time)
{
	if (!InUse()) return false;

	m_lifeTimeLeft -= time;
	m_state.live.x += m_state.live.xVel * time;
	m_state.live.y += m_state.live.yVel * time;

	return m_lifeTimeLeft < 0.0;
}

void Particle::Render(HDC& memDC, CoordinateData& cd) 
{
	if (!InUse()) return;

	double winX = m_state.live.x, winY = m_state.live.y;
	cd.ConvertCoordinateOpenGLToWindows(&winX, &winY);

	Rectangle(memDC,
		static_cast<int>((winX - m_state.live.xSize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY - m_state.live.ySize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winX + m_state.live.xSize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY + m_state.live.ySize / 2.0) * PIXEL_PER_METER)
	);
}

bool Particle::InUse() const
{
	return m_lifeTimeLeft > 0;
}

Particle* Particle::GetNext() const
{
	return m_state.next;
}

void Particle::SetNext(Particle* next)
{
	m_state.next = next;
}
