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
	m_x = x;
	m_y = y;
	m_xVel = xVel;
	m_yVel = yVel;
	m_lifeTimeLeft = lifeTime;
	m_xSize = DEFUALT_PARTICLE_SIZE;
	m_ySize = DEFUALT_PARTICLE_SIZE;
}

void Particle::Update(double time)
{
	if (!InUse()) return;

	m_lifeTimeLeft -= time;
	m_x += m_xVel * time;
	m_y += m_yVel * time;
}

void Particle::Render(HDC& memDC, CoordinateData& cd) 
{
	if (!InUse()) return;

	double winX = m_x, winY = m_y;
	cd.ConvertCoordinateOpenGLToWindows(&winX, &winY);

	Rectangle(memDC,
		static_cast<int>((winX - m_xSize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY - m_ySize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winX + m_xSize / 2.0) * PIXEL_PER_METER),
		static_cast<int>((winY + m_ySize / 2.0) * PIXEL_PER_METER)
	);
}

bool Particle::InUse() const
{
	return m_lifeTimeLeft > 0;
}
