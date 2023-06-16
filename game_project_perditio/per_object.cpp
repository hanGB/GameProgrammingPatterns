#include "stdafx.h"
#include "per_object.h"

PERObject::PERObject()
	: m_input(nullptr), m_ai(nullptr),
	m_physics(nullptr), m_graphics(nullptr)
{

}


PERObject::PERObject(
	PERInputComponent* input, PERAiComponent* ai, 
	PERPhysicsComponent* physics, PERGraphicsComponent* graphics)
	: m_input(input), m_ai(ai),
	m_physics(physics), m_graphics(graphics)
{

}

void PERObject::InitNullObject(PERInputComponent* input, PERAiComponent* ai, PERPhysicsComponent* physics, PERGraphicsComponent* graphics)
{
	m_input = input;
	m_ai = ai;
	m_physics = physics;
	m_graphics = graphics;
}

PERObject::~PERObject()
{
	delete m_input;
	delete m_ai;
	delete m_physics;
	delete m_graphics;
}

void PERObject::Update(PERController& controller, PERWorld& world, double dTime)
{
	m_input->Update(*this, controller);
	m_ai->Update(*this, dTime);
	m_physics->Update(*this, world, dTime);
	m_graphics->Update(*this, dTime);
}

void PERObject::Render(PERRenderer& renderer)
{
	m_graphics->Render(*this, renderer);
}

PERInputComponent& PERObject::GetInput()
{
	return *m_input;
}

PERAiComponent& PERObject::GetAi()
{
	return *m_ai;
}

PERPhysicsComponent& PERObject::GetPhysics()
{
	return *m_physics;
}

PERGraphicsComponent& PERObject::GetGraphcis()
{
	return *m_graphics;
}


PERVec3 PERObject::GetPosition() const
{
	return m_position;
}

PERVec2 PERObject::GetSize() const
{
	return m_size;
}

PERVec3 PERObject::GetVelocity() const
{
	return m_velocity;
}

PERVec3 PERObject::GetCurrentAccel() const
{
	return m_currentAccel;
}

double PERObject::GetMass() const
{
	return m_mass;
}

void PERObject::SetPosition(PERVec3 pos)
{
	m_position = pos;
}

void PERObject::SetSize(PERVec2 size)
{
	m_size = size;
}

void PERObject::SetVelocity(PERVec3 vel)
{
	m_velocity = vel;
}

void PERObject::SetCurrentAccel(PERVec3 acc)
{
	m_currentAccel = acc;
}

void PERObject::SetMass(double mass)
{
	m_mass = mass;
}

