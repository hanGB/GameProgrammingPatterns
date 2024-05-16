#include "stdafx.h"
#include "pressure_physics_componenet.h"
#include "per_world.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void PressurePhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	world.CheckCollision(*GetOwner(), audio, dTime);

	if (!m_isKeepPressed && m_isOn)
	{
		dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
		m_isOn = false;
	}
	m_isKeepPressed = false;

	PhysicsComponent::Update(world, audio, dTime);
}

void PressurePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	PhysicsComponent::SetData(data);
}

void PressurePhysicsComponent::Initialize()
{
	m_isKeepPressed = false;
	m_isOn = false;
	PhysicsComponent::Initialize();
}

void PressurePhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	m_isKeepPressed = true;
	if (m_isOn) return;

	dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
	m_isOn = true;

	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

