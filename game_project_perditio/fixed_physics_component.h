#pragma once
#include "per_physics_component.h"

class FixedPhysicsComponent : public PERPhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, double dTime);
};
