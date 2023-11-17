#include "stdafx.h"
#include "fixed_physics_component.h"

void FixedPhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
}

void FixedPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
}

void FixedPhysicsComponent::ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
}

void FixedPhysicsComponent::GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime)
{
}
