#pragma once
#include "comp_renderer.h"
#include "comp_world.h"
#include "input_component.h"
#include "physics_component.h"
#include "graphics_component.h"

class Bjorn {
public:
	Bjorn(InputComponent* input);
	~Bjorn();

	void Update(CompWorld& world, double elapsedTimeInSec);
	void Render(CompRenderer& renderer);

	CompVector3<double> GetPosition() const;
	CompVector2<double> GetCurrentAccel() const;
	CompVector2<double> GetVelocity() const;
	CompVector2<double> GetSize() const;
	bool GetIsFalling() const;
	double GetMaximumVelocityX() const;
	double GetMass() const;

	void SetPosition(CompVector3<double> position);
	void SetCurrentAccel(CompVector2<double> accel);
	void SetVelocity(CompVector2<double> velocity);
	void SetSize(CompVector2<double> size);
	void SetIsFalling(bool fall);
	void SetMass(double mass);

private:
	void InitCurrentAccel();

	static const int MAXIMUM_VELOCITY_X = 3;

	double m_maximumVelocityX = (double)MAXIMUM_VELOCITY_X;
	
	InputComponent* m_input;
	PhysicsComponent m_physics;
	GraphicsComponent m_graphics;

	CompVector3<double> m_position;
	CompVector2<double> m_velocity;
	CompVector2<double> m_currentAccel;
	CompVector2<double> m_size;
	double m_mass;

	bool m_isFalling;
};