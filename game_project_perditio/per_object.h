#pragma once
#include "per_controller.h"
#include "per_world.h"
#include "per_renderer.h"
#include "per_input_component.h"
#include "per_ai_component.h"
#include "per_physics_component.h"
#include "per_graphics_component.h"

class ObjectFactory;

class PERObject {
	friend class ObjectFactory;

public:
	~PERObject();

	void Update(PERController& controller, PERWorld& world, double dTime);
	void Render(PERRenderer& renderer);
	
	PERInputComponent& GetInput();
	PERAiComponent& GetAi();
	PERPhysicsComponent& GetPhysics();
	PERGraphicsComponent& GetGraphcis();

	// getter
	PERVec3 GetPosition() const;
	PERVec2 GetSize() const;
	PERVec3 GetVelocity() const;
	PERVec3 GetCurrentAccel() const;
	double GetMass() const;

	// setter
	void SetPosition(PERVec3 pos);
	void SetSize(PERVec2 size);
	void SetVelocity(PERVec3 vel);
	void SetCurrentAccel(PERVec3 acc);
	void SetMass(double mass);

	static const int c_MAXIMUM_VERTICAL_VELOCITY = 3;

private:
	PERObject();
	PERObject(PERInputComponent* input, PERAiComponent* ai, PERPhysicsComponent* physics, PERGraphicsComponent* graphics);

	void InitNullObject(PERInputComponent* input, PERAiComponent* ai, PERPhysicsComponent* physics, PERGraphicsComponent* graphics);

	// 컨포넌트
	PERInputComponent*		m_input;
	PERAiComponent*			m_ai;
	PERPhysicsComponent*	m_physics;
	PERGraphicsComponent*	m_graphics;

	// 정보
	PERVec3	m_position = PERVec3(0.0, 0.0, 0.0);
	PERVec2 m_size = PERVec2(0.5, 0.5);
	PERVec3 m_velocity = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_currentAccel = PERVec3(0.0, 0.0, 0.0);
	double	m_mass = 50.0;
};