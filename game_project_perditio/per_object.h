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

	PERInputComponent& GetInput();
	PERAiComponent& GetAi();
	PERPhysicsComponent& GetPhysics();
	PERGraphicsComponent& GetGraphics();

	// getter
	PERObjectType GetObjectType() const;

	PERVec3 GetPosition() const;
	PERVec3 GetSize() const;
	PERVec3 GetVelocity() const;
	PERVec3 GetCurrentAccel() const;
	double GetMass() const;

	int GetIDInWorld() const;

	// setter
	void SetPosition(PERVec3 pos);
	void SetSize(PERVec3 size);
	void SetVelocity(PERVec3 vel);
	void SetCurrentAccel(PERVec3 acc);
	void SetMass(double mass);

	void SetIDInWorld(int id);

	static const int c_MAXIMUM_VERTICAL_VELOCITY = 3;

private:
	PERObject(ObjectFactory& factory, PERInputComponent* input, PERAiComponent* ai, PERPhysicsComponent* physics, PERGraphicsComponent* graphics);

	// ÆÑÅä¸®
	ObjectFactory& m_factory;

	// ÄÁÆ÷³ÍÆ®
	PERInputComponent*		m_input;
	PERAiComponent*			m_ai;
	PERPhysicsComponent*	m_physics;
	PERGraphicsComponent*	m_graphics;

	// Á¤º¸
	PERVec3	m_position = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_size = PERVec3(0.5, 0.5, 0.5);
	PERVec3 m_velocity = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_currentAccel = PERVec3(0.0, 0.0, 0.0);
	double	m_mass = 50.0;

	int m_idInWorld = -1;
};