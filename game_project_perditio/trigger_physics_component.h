#pragma once
#include "physics_component.h"

class TriggerPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize();
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);

private:
	bool m_isOn = false;
};
