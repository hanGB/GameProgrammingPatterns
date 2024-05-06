#include "stdafx.h"
#include "fixed_physics_component.h"

void FixedPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
}

void FixedPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
}

void FixedPhysicsComponent::Initialize(PERComponent::PhysicsData data)
{
	SetData(data);
}

void FixedPhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
}

void FixedPhysicsComponent::GiveForce(PERWorld& world, PERVec3 force, double dTime)
{
}
