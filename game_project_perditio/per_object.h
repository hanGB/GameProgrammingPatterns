#pragma once
#include "per_controller.h"
#include "per_world.h"
#include "per_renderer.h"
#include "object_state.h"
#include "input_component.h"
#include "ai_component.h"
#include "physics_component.h"
#include "graphics_component.h"

class ObjectFactory;

enum class PERObjectType {
	PLAYER,
	FIXED_BLOCK,
	MOVABLE_BLOCK,
	MONSTER,
	BULLET,
	BLADE,
	SPAWNER,
	BUTTON,
	PRESSURE,
	DOOR,
	PARTICLE_EFFECTER,
	TRIGGER,
	NUM_OBJECT_TYPE
};

enum class PERBoundingType {
	RECTANGLE,
	CIRCLE,
	TRIANGLE,

	NUM_BOUNDING_TYPE
};

class PERObject {
	friend class ObjectFactory;

public:
	~PERObject();

	void Initialize();

	bool IsLifeTimeEnd(double dTime);

	ObjectState& GetObjectState();

	InputComponent& GetInput();
	AiComponent& GetAi();
	PhysicsComponent& GetPhysics();
	GraphicsComponent& GetGraphics();

	PERObjectType GetObjectType() const;
	
	PERObject* GetParent();

	PERVec3 GetPosition() const;
	PERVec3 GetSize() const;
	PERVec3 GetVelocity() const;
	PERVec3 GetCurrentAccel() const;
	double GetMass() const;
	PERBoundingType GetBoundingType() const;
	PERVec3 GetBoundingBoxSize() const;
	PERVec3 GetBoundingBoxPosition() const;

	PERVec3 GetCollidedVelocity() const;
	double GetCollidedMass() const;

	PERObject* GetCollidedObject();

	int GetIDInWorld() const;
	double GetLifeTime() const;

	// setter
	void SetParent(PERObject* object);
	
	void SetPosition(PERVec3 pos);
	void SetSize(PERVec3 size);
	void SetVelocity(PERVec3 vel);
	void SetCurrentAccel(PERVec3 acc);
	void SetMass(double mass);
	void SetBoundingType(PERBoundingType bounding);
	void SetBoundingBox(PERVec3 relativeSize, PERVec3 relativePos);

	void SetIDInWorld(int id);
	void SetLifeTime(double time);

	void SetCollidedObject(PERObject* object, PERVec3 collidedMomentVel);

	// 현재 위치를 스폰 위치로 설정
	void SetCurrentPositionToSpawnPosition();

	// 잠들어야 할 
	bool IsHaveToSleep(const PERVec3& playerPos);

	static const int c_MAXIMUM_XY_VELOCITY = 3;

private:
	PERObject(ObjectFactory& factory, ObjectState* m_objectState,
		InputComponent* input, AiComponent* ai, PhysicsComponent* physics, GraphicsComponent* graphics);

	void SetSleepDistance();

	// 팩토리
	ObjectFactory& m_factory;

	// 상태
	ObjectState* m_objectState;

	// 컨포넌트
	InputComponent*		m_input;
	AiComponent*			m_ai;
	PhysicsComponent*	m_physics;
	GraphicsComponent*	m_graphics;

	// 부모
	PERObject* m_parent = nullptr;

	// 물리 정보
	PERVec3	m_position	= PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_size		= PERVec3(0.5, 0.5, 0.5);
	PERVec3 m_velocity	= PERVec3(0.0, 0.0, 0.0);
	PERVec3 m_currentAccel	= PERVec3(0.0, 0.0, 0.0);
	double	m_mass			= 50.0;
	// 충돌체 정보
	PERBoundingType m_boundingType			= PERBoundingType::RECTANGLE;
	PERVec3			m_boundingBoxRelativeSize		= PERVec3(1.0, 1.0, 1.0);
	PERVec3			m_boundingBoxRelativePosition	= PERVec3(0.0, 0.0, 0.0);

	// 충돌된 오브젝트
	PERObject* m_collidedObject = nullptr;
	PERVec3 m_collidedMomentVelocity;
	// 월드 내 정보
	int m_idInWorld = -1;
	double m_lifeTime = PER_MAXIMUM_LIFE_TIME;

	// 잠드는 거리
	double m_sleepDistance = PER_SMALL_OBJECT_SLEEP_DISTANCE;
};