#pragma once
#include "physics_component.h"

class MovablePhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime);
	virtual void SetData(PERComponent::PhysicsData data);

private:
	void Move(PERObject& object, double dTime);
	void MoveWithoutFriction(PERObject& object, double dTime);

	std::function<void(MovablePhysicsComponent&, PERObject&, double)> m_MoveFunc;
};
