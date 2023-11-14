#pragma once
#include "per_controller.h"
#include "per_world.h"
#include "per_renderer.h"
#include "object_state.h"
#include "input_component.h"
#include "ai_component.h"
#include "physics_component.h"
#include "graphics_component.h"
#include "ui_element.h"

class ObjectFactory;

class PERObject {
	friend class ObjectFactory;

public:
	~PERObject();

	bool IsLifeTimeIsEnd(double dTime);

	ObjectState& GetObjectState();

	InputComponent& GetInput();
	AiComponent& GetAi();
	PhysicsComponent& GetPhysics();
	GraphicsComponent& GetGraphics();

	// getter
	UiElement* GetFloatingUi();

	PERObjectType GetObjectType() const;
	
	PERObject* GetParent();

	PERVec3 GetPosition() const;
	PERVec3 GetSize() const;
	PERVec3 GetVelocity() const;
	PERVec3 GetCurrentAccel() const;
	double GetMass() const;
	PERBoundingType GetBoundingType() const;

	PERVec3 GetCollidedVelocity() const;
	double GetCollidedMass() const;

	PERObject* GetCollidedObject();

	int GetIDInWorld() const;

	// setter
	void SetFlotingUi(UiElement* element);

	void SetParent(PERObject* object);
	
	void SetPosition(PERVec3 pos);
	void SetSize(PERVec3 size);
	void SetVelocity(PERVec3 vel);
	void SetCurrentAccel(PERVec3 acc);
	void SetMass(double mass);
	void SetBoundingType(PERBoundingType bounding);

	void SetIDInWorld(int id);
	void SetLifeTime(double time);

	void SetCollidedObject(PERObject* object, PERVec3 collidedMomentVel);

	// 현재 위치를 스폰 위치로 설정
	void SetCurrentPositionToSpawnPosition();

	static const int c_MAXIMUM_XY_VELOCITY = 3;

private:
	PERObject(ObjectFactory& factory, ObjectState* m_objectState, UiElement* floatingUi,
		InputComponent* input, AiComponent* ai, PhysicsComponent* physics, GraphicsComponent* graphics);

	// 팩토리
	ObjectFactory& m_factory;

	// 상태
	ObjectState* m_objectState;

	// 컨포넌트
	InputComponent*		m_input;
	AiComponent*			m_ai;
	PhysicsComponent*	m_physics;
	GraphicsComponent*	m_graphics;

	// 오브젝트 위에 떠있는 ui
	UiElement* m_flotingUi = nullptr;

	// 부모
	PERObject* m_parent = nullptr;

	// 정보
	PERVec3	m_position = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_size = PERVec3(0.5, 0.5, 0.5);
	PERVec3 m_velocity = PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_currentAccel = PERVec3(0.0, 0.0, 0.0);
	double	m_mass = 50.0;
	PERBoundingType m_boundingType = PERBoundingType::RECTANGLE;

	// 충돌된 오브젝트
	PERObject* m_collidedObject = nullptr;
	PERVec3 m_collidedMomentVelocity;

	int m_idInWorld = -1;
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;
};