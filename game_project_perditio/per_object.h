#pragma once
#include "per_controller.h"
#include "per_world.h"
#include "per_renderer.h"
#include "input_component.h"
#include "ai_component.h"
#include "physics_component.h"
#include "graphics_component.h"

class ObjectFactory;

class PERObject {
	friend class ObjectFactory;

public:
	~PERObject();

	bool IsLifeTimeIsEnd(double dTime);

	InputComponent& GetInput();
	AiComponent& GetAi();
	PhysicsComponent& GetPhysics();
	GraphicsComponent& GetGraphics();

	// getter
	PERObjectType GetObjectType() const;
	
	PERObject* GetParent();

	PERVec3 GetPosition() const;
	PERVec3 GetSize() const;
	PERVec3 GetVelocity() const;
	PERVec3 GetCurrentAccel() const;
	double GetMass() const;
	PERBoundingType GetBoundingType() const;

	int GetIDInWorld() const;

	// setter
	void SetParent(PERObject* object);

	void SetPosition(PERVec3 pos);
	void SetSize(PERVec3 size);
	void SetVelocity(PERVec3 vel);
	void SetCurrentAccel(PERVec3 acc);
	void SetMass(double mass);
	void SetBoundingType(PERBoundingType bounding);

	void SetIDInWorld(int id);
	void SetLifeTime(double time);

	static const int c_MAXIMUM_XY_VELOCITY = 3;

private:
	PERObject(ObjectFactory& factory, InputComponent* input, AiComponent* ai, PhysicsComponent* physics, GraphicsComponent* graphics);

	// 팩토리
	ObjectFactory& m_factory;

	// 컨포넌트
	InputComponent*		m_input;
	AiComponent*			m_ai;
	PhysicsComponent*	m_physics;
	GraphicsComponent*	m_graphics;

	// 부모
	PERObject* m_parent = nullptr;

	// 정보
	PERVec3	m_position = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_size = PERVec3(0.5, 0.5, 0.5);
	PERVec3 m_velocity = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_currentAccel = PERVec3(0.0, 0.0, 0.0);
	double	m_mass = 50.0;
	PERBoundingType m_boundingType = PERBoundingType::RECTANGLE;
	
	int m_idInWorld = -1;
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;
};