#pragma once
#include "physics_component.h"

class FixedPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void ProcessCollision(PERObject& myObject, PERVec3 otherPos, PERVec3 otherSize, PERVec3 otherVel, PERBoundingType otherType);
};
