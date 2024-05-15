#pragma once
#include "physics_component.h"

class StuckPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize();
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);

	PERVec3 GetStuckPosition() const;

private:
	PERVec3 m_stuckPosition = PERVec3(0.0, 0.0, 0.0);
};
