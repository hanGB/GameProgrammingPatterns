#include "stdafx.h"
#include "bjorn.h"

Bjorn::Bjorn()
{
	m_posX = 0.0; m_posY = 0.0;
	m_sizeX = 0.8; m_sizeY = 0.8f;
	m_velocityX = 0.0; m_velocityY = 0.0;
	m_mass = 5.0;
	m_isFalling = true;

	m_idleColor.red = 0; m_idleColor.green = 127; m_idleColor.blue = 127;
	m_walkRightColor.red = 0; m_walkRightColor.green = 0; m_walkRightColor.blue = 127;
	m_walkLeftColor.red = 0; m_walkLeftColor.green = 127; m_walkLeftColor.blue = 0;

	m_color = m_idleColor;
}

Bjorn::~Bjorn()
{
}

void Bjorn::Update(CompWorld& world, double elapsedTimeInSec)
{
	InitCurrentAccel();

	m_input.Update(*this);

	double t = elapsedTimeInSec;

	// 이동 관련
	if (fabs(m_velocityX) > 0.f && !m_isFalling) {
		// 마찰
		m_currentAccX += m_velocityX / std::abs(m_velocityX) * GPP_FRICTION * (-GPP_GRAVITY);

		double tempVelocityX = m_velocityX + m_currentAccX * t;
		if (tempVelocityX * m_velocityX < 0.0f) {
			m_velocityX = 0.0f;
		}
		else {
			m_velocityX = tempVelocityX;
		}

		m_velocityX += m_currentAccX * t;
	}
	else if (m_isFalling) {
		m_currentAccX = 0.0;
		m_currentAccY -= GPP_GRAVITY;
		m_velocityY += m_currentAccY * t;
	}
	else {
		m_velocityX += m_currentAccX * t;
	}

	m_posX += (m_velocityX * t + 0.5f * m_currentAccX * t * t);
	m_posY += (m_velocityY * t + 0.5f * m_currentAccY * t * t);

	m_isFalling = !world.CheckCollision(&m_posX, &m_posY, m_sizeX, m_sizeY);

	if (m_velocityX > STOP_VELOCITY) {
		m_color = m_walkRightColor;
	}
	else if (m_velocityX < -STOP_VELOCITY) {
		m_color = m_walkLeftColor;
	}
	else {
		m_color = m_idleColor;
	}

}

void Bjorn::Render(CompRenderer& renderer)
{
	renderer.RenderEllipse(m_posX, m_posY, m_sizeX, m_sizeY, m_color);
}

void Bjorn::GetCurrentAcc(double* x, double* y)
{
	*x = m_currentAccX;
	*y = m_currentAccY;
}

void Bjorn::GetVelocity(double* x, double* y)
{
	*x = m_velocityX;
	*y = m_velocityY;
}

bool Bjorn::GetIsFalling() const
{
	return m_isFalling;
}

double Bjorn::GetMaximumVelocityX() const
{
	return m_maximumVelocityX;
}

double Bjorn::GetMass() const
{
	return m_mass;
}

void Bjorn::SetCurrentAcc(double x, double y)
{
	m_currentAccX = x;
	m_currentAccY = y;
}

void Bjorn::SetVelocity(double x, double y)
{
	m_velocityX = x;
	m_velocityY = y;
}

void Bjorn::SetIsFalling(bool fall)
{
	m_isFalling = fall;
}

void Bjorn::SetMass(double mass)
{
	m_mass = mass;
}

void Bjorn::InitCurrentAccel()
{
	m_currentAccX = 0.0;
	m_currentAccY = 0.0;
}
