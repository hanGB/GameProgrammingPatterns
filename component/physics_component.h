#pragma once

class CompObject;
class CompWorld;

class PhysicsComponent {
public:
	virtual ~PhysicsComponent() {}
	virtual void Update(CompObject& object, CompWorld& world, double elapsedTimeInSec) = 0;
};

class BjornPhysicsComponent : public PhysicsComponent {
public:
	virtual void Update(CompObject& object, CompWorld& world, double elapsedTimeInSec);

private:
	CompVector2<double> m_volume = { 0.8, 0.8 };
};