#include "stdafx.h"
#include "comp_object.h"

CompObject::CompObject(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics)
	: m_input(input), m_physics(physics), m_graphics(graphics)
{
	m_position = { 0.0, 0.0, 0.0 };
	m_velocity = { 0.0, 0.0 };
	m_currentAccel = { 0.0, 0.0 };
	m_size = { 1.0, 1.0 };
	m_mass = { 100.0 };

	m_isFalling = true;
}

CompObject::~CompObject()
{
	delete m_input;
	delete m_physics;
	delete m_graphics;
}

void CompObject::Update(CompWorld& world, double elapsedTimeInSec)
{
	InitCurrentAccel();

	m_input->Update(*this);
	m_physics->Update(*this, world, elapsedTimeInSec);
	m_graphics->Update(*this);
}

void CompObject::Render(CompRenderer& renderer)
{
	m_graphics->Render(*this, renderer);
}

CompVector3<double> CompObject::GetPosition() const
{
	return m_position;
}

CompVector2<double> CompObject::GetCurrentAccel() const
{
	return m_currentAccel;
}

CompVector2<double> CompObject::GetVelocity() const
{
	return m_velocity;
}

CompVector2<double> CompObject::GetSize() const
{
	return m_size;
}

bool CompObject::GetIsFalling() const
{
	return m_isFalling;
}

double CompObject::GetMaximumVelocityX() const
{
	return m_maximumVelocityX;
}

double CompObject::GetMass() const
{
	return m_mass;
}

void CompObject::SetPosition(CompVector3<double> position)
{
	m_position = position;
}

void CompObject::SetCurrentAccel(CompVector2<double> accel)
{
	m_currentAccel = accel;
}

void CompObject::SetVelocity(CompVector2<double> velocity)
{
	m_velocity = velocity;
}

void CompObject::SetSize(CompVector2<double> size)
{
	m_size = size;
}

void CompObject::SetIsFalling(bool fall)
{
	m_isFalling = fall;
}

void CompObject::SetMass(double mass)
{
	m_mass = mass;
}

void CompObject::InitCurrentAccel()
{
	m_currentAccel.x = 0.0;
	m_currentAccel.y = 0.0;
}
