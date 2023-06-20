#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;

class PERPhysicsComponent {
public:
	virtual ~PERPhysicsComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, double dTime) = 0;
	virtual void SetData(PERComponent::PhysicsData data) = 0;
};