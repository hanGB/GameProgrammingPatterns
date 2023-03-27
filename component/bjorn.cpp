#include "stdafx.h"
#include "bjorn.h"

Bjorn::Bjorn()
{
	m_position = { 0.0, 0.0, 0.0 };
	m_velocity = { 0.0, 0.0 };
	m_mass = 5.0;
	m_isFalling = true;
	m_size = { 0.8, 0.8 };
	m_physics.SetVolume(m_size);

	m_idleColor = { 0, 127, 127 };
	m_walkRightColor = { 0, 0, 127 };
	m_walkLeftColor = { 0, 127, 0 };

	m_color = m_idleColor;
}

Bjorn::~Bjorn()
{
}

void Bjorn::Update(CompWorld& world, double elapsedTimeInSec)
{
	InitCurrentAccel();

	m_input.Update(*this);
	m_physics.Update(*this, world, elapsedTimeInSec);

	if (m_velocity.x > STOP_VELOCITY) {
		m_color = m_walkRightColor;
	}
	else if (m_velocity.x < -STOP_VELOCITY) {
		m_color = m_walkLeftColor;
	}
	else {
		m_color = m_idleColor;
	}
}

void Bjorn::Render(CompRenderer& renderer)
{
	renderer.RenderEllipse(m_position, m_size, m_color);
}

CompVector3<double> Bjorn::GetPosition() const
{
	return m_position;
}

CompVector2<double> Bjorn::GetCurrentAccel() const
{
	return m_currentAccel;
}

CompVector2<double> Bjorn::GetVelocity() const
{
	return m_velocity;
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

void Bjorn::SetPosition(CompVector3<double> position)
{
	m_position = position;
}

void Bjorn::SetCurrentAccel(CompVector2<double> accel)
{
	m_currentAccel = accel;
}

void Bjorn::SetVelocity(CompVector2<double> velocity)
{
	m_velocity = velocity;
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
	m_currentAccel.x = 0.0;
	m_currentAccel.y = 0.0;
}
