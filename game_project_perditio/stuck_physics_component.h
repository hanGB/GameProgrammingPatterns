#pragma once
#include "physics_component.h"

class StuckPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
	virtual void GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime);

	PERVec3 GetStuckPosition() const;

private:
	PERVec3 m_stuckPosition = PERVec3(0.0, 0.0, 0.0);
};
