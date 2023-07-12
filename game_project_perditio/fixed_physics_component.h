#pragma once
#include "per_physics_component.h"

class FixedPhysicsComponent : public PERPhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
};
