#pragma once
#include "per_physics_component.h"

class PlayerPhysicsComponent : public PERPhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, double dTime);

private:
	void Move(PERObject& object, double dTime);
};
