#include "stdafx.h"
#include "per_object.h"
#include "object_factory.h"

PERObject::PERObject(ObjectFactory& factory, ObjectState* objectState,
	InputComponent* input, AiComponent* ai, 
	PhysicsComponent* physics, GraphicsComponent* graphics)
	: m_factory(factory), m_objectState(objectState),
	m_input(input), m_ai(ai), m_physics(physics), m_graphics(graphics)
{
	m_size = m_factory.GetSize();
	m_mass = m_factory.GetMass();

	// 컨포넌트 설정
	m_input->SetData(m_factory.GetInputData());
	m_ai->SetData(m_factory.GetAiData());
	m_physics->SetData(m_factory.GetPhysicsData());
	m_graphics->SetData(m_factory.GetGraphicsData());
}

PERObject::~PERObject()
{
	delete m_objectState;
	delete m_input;
	delete m_ai;
	delete m_physics;
	delete m_graphics;
}

bool PERObject::IsLifeTimeIsEnd(double dTime)
{
	m_lifeTime -= dTime;

	return  0.0 > m_lifeTime;
}

ObjectState& PERObject::GetObjectState()
{
	return *m_objectState;
}

InputComponent& PERObject::GetInput()
{
	return *m_input;
}

AiComponent& PERObject::GetAi()
{
	return *m_ai;
}

PhysicsComponent& PERObject::GetPhysics()
{
	return *m_physics;
}

GraphicsComponent& PERObject::GetGraphics()
{
	return *m_graphics;
}

PERObjectType PERObject::GetObjectType() const
{
	return m_factory.GetObjectType();
}

PERObject* PERObject::GetParent()
{
	return m_parent;
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

PERBoundingType PERObject::GetBoundingType() const
{
	return m_boundingType;
}

PERVec3 PERObject::GetCollidedVelocity() const
{
	if (m_collidedObject) 
		return PERVec3(
			m_velocity.x + m_collidedMomentVelocity.x,
			m_velocity.y + m_collidedMomentVelocity.y,
			m_velocity.z + m_collidedMomentVelocity.z
		);

	return m_velocity;
}

double PERObject::GetCollidedMass() const
{
	if (m_collidedObject) return m_mass + m_collidedObject->GetMass();

	return m_mass;
}

PERObject* PERObject::GetCollidedObject()
{
	return m_collidedObject;
}

int PERObject::GetIDInWorld() const
{
	return m_idInWorld;
}

double PERObject::GetLifeTime() const
{
	return m_lifeTime;
}

void PERObject::SetParent(PERObject* object)
{
	m_parent = object;
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

void PERObject::SetBoundingType(PERBoundingType bounding)
{
	m_boundingType = bounding;
}

void PERObject::SetIDInWorld(int id)
{
	m_idInWorld = id;
}

void PERObject::SetLifeTime(double time)
{
	m_lifeTime = time;
}

void PERObject::SetCollidedObject(PERObject* object, PERVec3 collidedMomentVel)
{
	m_collidedObject = object;
	m_collidedMomentVelocity = collidedMomentVel;
}

void PERObject::SetCurrentPositionToSpawnPosition()
{
	m_objectState->SetSpawnPosition(m_position);
}
