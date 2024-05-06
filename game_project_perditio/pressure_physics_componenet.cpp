#include "stdafx.h"
#include "pressure_physics_componenet.h"
#include "per_world.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void PressurePhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	world.CheckCollision(*GetOwner(), dTime);

	if (!m_isKeepPressed && m_isOn)
	{
		dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
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

void PressurePhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	m_isKeepPressed = true;
	if (m_isOn) return;

	dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
	m_isOn = true;
}

void PressurePhysicsComponent::GiveForce(PERWorld& world, PERVec3 force, double dTime)
{
}

