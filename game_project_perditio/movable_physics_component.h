#pragma once
#include "per_physics_component.h"

class MovablePhysicsComponent : public PERPhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);

private:
	void Move(PERObject& object, double dTime);
};
