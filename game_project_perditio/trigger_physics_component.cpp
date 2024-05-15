#include "stdafx.h"
#include "trigger_physics_component.h"
#include "per_world.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void TriggerPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	if (!world.CheckCollision(*GetOwner(), dTime)) m_isOn = false;

	PhysicsComponent::Update(world, audio, dTime);
}

void TriggerPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	PhysicsComponent::SetData(data);
}

void TriggerPhysicsComponent::Initialize()
{
	m_isOn = false;
	PhysicsComponent::Initialize();
}

void TriggerPhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	if (collidedObject.GetObjectType() != PERObjectType::PLAYER) return;
	if (m_isOn) return;

	dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
	m_isOn = true;

	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

