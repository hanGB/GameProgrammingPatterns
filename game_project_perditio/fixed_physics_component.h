#pragma once
#include "physics_component.h"

class FixedPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize(PERComponent::PhysicsData data);
	virtual void ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
	virtual void GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime);
};
