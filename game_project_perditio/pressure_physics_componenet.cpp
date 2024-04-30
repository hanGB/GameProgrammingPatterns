#include "stdafx.h"
#include "pressure_physics_componenet.h"
#include "per_world.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void PressurePhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	if (!world.CheckCollision(object, dTime)) m_isKeepPressed = false;
}

void PressurePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
}

void PressurePhysicsComponent::ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	if (otherObject.GetObjectType() != PERObjectType::PLAYER) return;

	if (m_isKeepPressed) return;

	dynamic_cast<MakingSignalAiComponent*>(&myObject.GetAi())->SetIsGetInput(true);
	m_isKeepPressed = true;
}

void PressurePhysicsComponent::GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime)
{
}

