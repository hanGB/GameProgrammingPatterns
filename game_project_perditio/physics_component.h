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
	virtual void ProcessCollision(PERObject& myObject, PERVec3 otherPos, PERVec3 otherSize, PERVec3 otherVel, PERBoundingType otherType) = 0;
};