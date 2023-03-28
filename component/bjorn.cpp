#include "stdafx.h"
#include "bjorn.h"

Bjorn::Bjorn(InputComponent* input)
	: m_input(input)
{
	m_position = { 0.0, 0.0, 0.0 };
	m_velocity = { 0.0, 0.0 };
	m_size = { 0.8, 0.8 };
	m_mass = 5.0;
	m_isFalling = true;

	m_physics.SetVolume(m_size);
	m_graphics.SetShapeType(CompShapeType::COMP_SHAPE_TYPE_ELLIPSE);
	m_graphics.SetColors({ 0, 127, 127 }, { 0, 127, 0 }, { 0, 0, 127 });
}

Bjorn::~Bjorn()
{
	delete m_input;
}

void Bjorn::Update(CompWorld& world, double elapsedTimeInSec)
{
	InitCurrentAccel();

	m_input->Update(*this);
	m_physics.Update(*this, world, elapsedTimeInSec);
	m_graphics.Update(*this);
}

void Bjorn::Render(CompRenderer& renderer)
{
	m_graphics.Render(*this, renderer);
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

CompVector2<double> Bjorn::GetSize() const
{
	return m_size;
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

void Bjorn::SetSize(CompVector2<double> size)
{
	m_size = size;
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
