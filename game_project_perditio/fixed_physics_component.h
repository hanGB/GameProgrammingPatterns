#pragma once
#include "physics_component.h"

class FixedPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize();
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
};
