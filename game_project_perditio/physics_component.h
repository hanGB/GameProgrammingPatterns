#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;

class PhysicsComponent {
public:
	virtual ~PhysicsComponent();

	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize();
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
	virtual void GiveForce(PERWorld& world, PERVec3 force, double dTime);

	void SetOwner(PERObject* object);
	void SetNextComponent(PhysicsComponent* component);
	PERObject* GetOwner();
	PhysicsComponent* GetNextComponent();

private:
	PERObject* m_owner;
	PhysicsComponent* m_nextComponent = nullptr;
};