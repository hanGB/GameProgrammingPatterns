#pragma once
#include "physics_component.h"

class MovablePhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);
	virtual void Initialize();
	virtual void ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime);
	virtual void GiveForce(PERWorld& world, PERObject& opponentObject, PERVec3 force, double dTime);

private:
	void Move(double dTime);
	void MoveWithoutFriction(double dTime);

	std::function<void(MovablePhysicsComponent&, double)> m_MoveFunc;
};
