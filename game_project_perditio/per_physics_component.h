#pragma once
class PERObject;
class PERWorld;

class PERPhysicsComponent {
public:
	virtual ~PERPhysicsComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, double dTime) = 0;
};