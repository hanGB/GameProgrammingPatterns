#include "stdafx.h"
#include "fixed_physics_component.h"

void FixedPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	PhysicsComponent::Update(world, audio, dTime);
}

void FixedPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	PhysicsComponent::SetData(data);
}

void FixedPhysicsComponent::Initialize()
{
	PhysicsComponent::Initialize();
}

void FixedPhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

void FixedPhysicsComponent::GiveForce(PERWorld& world, PERObject& opponentObject, PERVec3 force, double dTime)
{
	PhysicsComponent::GiveForce(world, opponentObject, force, dTime);
}
