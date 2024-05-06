#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;

class PhysicsComponent {
public:
	virtual ~PhysicsComponent() {}

	virtual void Update(PERWorld& world, PERAudio& audio, double dTime) = 0;
	virtual void SetData(PERComponent::PhysicsData data) = 0;
	virtual void Initialize(PERComponent::PhysicsData data) = 0;
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime) = 0;
	virtual void GiveForce(PERWorld& world, PERVec3 force, double dTime) = 0;

	void SetOwner(PERObject* object)
	{
		m_owner = object;
	}
	PERObject* GetOwner()
	{
		return m_owner;
	}

private:
	PERObject* m_owner;
};