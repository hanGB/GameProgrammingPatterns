#pragma once
#include "physics_component.h"

class PressurePhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize(PERComponent::PhysicsData data);
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
	virtual void GiveForce(PERWorld& world, PERVec3 force, double dTime);

private:
	bool m_isKeepPressed = false;
	bool m_isOn = false;
};
