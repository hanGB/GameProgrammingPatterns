#include "stdafx.h"
#include "per_object.h"
#include "object_factory.h"

PERObject::PERObject(ObjectFactory& factory,
	PERInputComponent* input, PERAiComponent* ai, 
	PERPhysicsComponent* physics, PERGraphicsComponent* graphics)
	: m_factory(factory),
	m_input(input), m_ai(ai),
	m_physics(physics), m_graphics(graphics)
{
	m_size = m_factory.GetSize();
	m_mass = m_factory.GetMass();

	// ������Ʈ ����
	m_input->SetData(m_factory.GetInputData());
	m_ai->SetData(m_factory.GetAiData());
	m_physics->SetData(m_factory.GetPhysicsData());
	m_graphics->SetData(m_factory.GetGraphicsData());
}

PERObject::~PERObject()
{
	delete m_input;
	delete m_ai;
	delete m_physics;
	delete m_graphics;
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

PERGraphicsComponent& PERObject::GetGraphics()
{
	return *m_graphics;
}


PERObjectType PERObject::GetObjectType() const
{
	return m_factory.GetObjectType();
}

PERVec3 PERObject::GetPosition() const
{
	return m_position;
}

PERVec3 PERObject::GetSize() const
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

int PERObject::GetIDInWorld() const
{
	return m_idInWorld;
}

void PERObject::SetPosition(PERVec3 pos)
{
	m_position = pos;
}

void PERObject::SetSize(PERVec3 size)
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

void PERObject::SetIDInWorld(int id)
{
	m_idInWorld = id;
}

