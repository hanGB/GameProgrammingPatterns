#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;

class PhysicsComponent {
public:
	virtual ~PhysicsComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime) = 0;
	virtual void SetData(PERComponent::PhysicsData data) = 0;
	virtual void ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime) = 0;
};