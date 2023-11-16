#pragma once
#include "physics_component.h"

class StuckPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);

private:
	PERVec3 m_stuckPosition = PERVec3(0.0, 0.0, 0.0);
};
