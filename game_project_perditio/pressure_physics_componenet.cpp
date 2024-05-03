#include "stdafx.h"
#include "pressure_physics_componenet.h"
#include "per_world.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void PressurePhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	world.CheckCollision(object, dTime);

	if (!m_isKeepPressed && m_isOn)
	{
		dynamic_cast<MakingSignalAiComponent*>(&object.GetAi())->SetIsGetInput(true);
		m_isOn = false;
	}
	m_isKeepPressed = false;
}

void PressurePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
}

void PressurePhysicsComponent::Initialize(PERComponent::PhysicsData data)
{
	m_isKeepPressed = false;
	m_isOn = false;
	SetData(data);
}

void PressurePhysicsComponent::ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	m_isKeepPressed = true;
	if (m_isOn) return;

	dynamic_cast<MakingSignalAiComponent*>(&myObject.GetAi())->SetIsGetInput(true);
	m_isOn = true;
}

void PressurePhysicsComponent::GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime)
{
}

