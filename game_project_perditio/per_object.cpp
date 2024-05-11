#include "stdafx.h"
#include "per_object.h"
#include "object_factory.h"

PERObject::PERObject(ObjectFactory& factory, ObjectState* objectState,
	InputComponent* input, AiComponent* ai, 
	PhysicsComponent* physics, GraphicsComponent* graphics)
	: m_factory(factory), m_objectState(objectState),
	m_input(input), m_ai(ai), m_physics(physics), m_graphics(graphics)
{
	// 스테이트와 컨포넌트의 오너를 자신으로 설정
	m_objectState->SetOwner(this);
	m_input->SetOwner(this);
	m_ai->SetOwner(this);
	m_physics->SetOwner(this);
	m_graphics->SetOwner(this);

	Initialize();
}

PERObject::~PERObject()
{
	delete m_objectState;
	delete m_input;
	delete m_ai;
	delete m_physics;
	delete m_graphics;
}

void PERObject::Initialize()
{
	// 컨포넌트 초기화
	m_input->Initialize();
	m_input->SetData(m_factory.GetInputData());
	m_ai->Initialize();
	m_ai->SetData(m_factory.GetAiData());
	m_physics->Initialize();
	m_physics->SetData(m_factory.GetPhysicsData());
	m_graphics->Initialize();
	m_graphics->SetData(m_factory.GetGraphicsData());

	// 오브젝트 스테이트 초기화
	m_factory.InitializeObjectState(m_objectState);

	// 오브젝트 클래스내 멤버 변수 초기화
	// 부모
	PERObject* m_parent = nullptr;

	// 물리 정보
	m_position = PERVec3(0.0, 0.0, 0.0);
	m_size = m_factory.GetSize();
	m_velocity = PERVec3(0.0, 0.0, 0.0);
	m_currentAccel = PERVec3(0.0, 0.0, 0.0);
	m_mass = m_factory.GetMass();

	// 충돌체 정보
	m_boundingType = PERBoundingType::RECTANGLE;
	m_boundingBoxRelativeSize = PERVec3(1.0, 1.0, 1.0);
	m_boundingBoxRelativePosition = PERVec3(0.0, 0.0, 0.0);

	// 충돌 정보
	m_collidedObject = nullptr;
	m_collidedMomentVelocity = PERVec3(0.0, 0.0, 0.0);

	// 월드 내 정보
	m_idInWorld = -1;
	m_lifeTime = PER_MAXIMUM_LIFE_TIME;

	// 슬립 거리 설정
	SetSleepDistance();
}

bool PERObject::IsLifeTimeEnd(double dTime)
{
	m_lifeTime -= dTime;

	return  0.0 > m_lifeTime;
}

ObjectState& PERObject::GetObjectState()
{
	return *m_objectState;
}

InputComponent& PERObject::GetInput()
{
	return *m_input;
}

AiComponent& PERObject::GetAi()
{
	return *m_ai;
}

PhysicsComponent& PERObject::GetPhysics()
{
	return *m_physics;
}

GraphicsComponent& PERObject::GetGraphics()
{
	return *m_graphics;
}

PERObjectType PERObject::GetObjectType() const
{
	return m_factory.GetObjectType();
}

PERObject* PERObject::GetParent()
{
	return m_parent;
}

PERVec3 PERObject::GetPosition() const
{
	return m_position;
}

PERVec3 PERObject::GetSize() const
{
	return m_size;
}

PERVec3 PERObject::GetVelocity() const
{
	return m_velocity;
}

PERVec3 PERObject::GetCurrentAccel() const
{
	return m_currentAccel;
}

double PERObject::GetMass() const
{
	return m_mass;
}

PERBoundingType PERObject::GetBoundingType() const
{
	return m_boundingType;
}

PERVec3 PERObject::GetBoundingBoxSize() const
{
	return PERVec3(m_size.x * m_boundingBoxRelativeSize.x, 
		m_size.y * m_boundingBoxRelativeSize.y, 
		m_size.z * m_boundingBoxRelativeSize.z);
}

PERVec3 PERObject::GetBoundingBoxPosition() const
{
	return PERVec3(m_position.x + m_boundingBoxRelativePosition.x, 
		m_position.y + m_boundingBoxRelativePosition.y, 
		m_position.z + m_boundingBoxRelativePosition.z);
}

PERVec3 PERObject::GetCollidedVelocity() const
{
	if (m_collidedObject) 
		return PERVec3(
			m_velocity.x + m_collidedMomentVelocity.x,
			m_velocity.y + m_collidedMomentVelocity.y,
			m_velocity.z + m_collidedMomentVelocity.z
		);

	return m_velocity;
}

double PERObject::GetCollidedMass() const
{
	if (m_collidedObject) return m_mass + m_collidedObject->GetMass();

	return m_mass;
}

PERObject* PERObject::GetCollidedObject()
{
	return m_collidedObject;
}

int PERObject::GetIDInWorld() const
{
	return m_idInWorld;
}

double PERObject::GetLifeTime() const
{
	return m_lifeTime;
}

void PERObject::SetParent(PERObject* object)
{
	m_parent = object;
}

void PERObject::SetPosition(PERVec3 pos)
{
	m_position = pos;
}

void PERObject::SetSize(PERVec3 size)
{
	m_size = size;

	SetSleepDistance();
}

void PERObject::SetSleepDistance()
{
	if (GetObjectType() == PERObjectType::BULLET)
	{
		m_sleepDistance = PER_BULLET_OBJECT_SLEEP_DISTANCE;
		return;
	}

	// 크기에 따른 슬립 거리 설정
	if ( m_size.x > PER_SMALL_OBJECT_SIZE_LIMIT || m_size.y > PER_SMALL_OBJECT_SIZE_LIMIT )
	{
		m_sleepDistance = PER_BIG_OBJECT_SLEEP_DISTANCE;
	}
	else
	{
		m_sleepDistance = PER_SMALL_OBJECT_SLEEP_DISTANCE;
	}
}

void PERObject::SetVelocity(PERVec3 vel)
{
	m_velocity = vel;
}

void PERObject::SetCurrentAccel(PERVec3 acc)
{
	m_currentAccel = acc;
}

void PERObject::SetMass(double mass)
{
	m_mass = mass;
}

void PERObject::SetBoundingType(PERBoundingType bounding)
{
	m_boundingType = bounding;
}

void PERObject::SetBoundingBox(PERVec3 relativeSize, PERVec3 relativePos)
{
	m_boundingBoxRelativeSize = relativeSize;
	m_boundingBoxRelativePosition = relativePos;
}

void PERObject::SetIDInWorld(int id)
{
	m_idInWorld = id;
}

void PERObject::SetLifeTime(double time)
{
	m_lifeTime = time;
}

void PERObject::SetCollidedObject(PERObject* object, PERVec3 collidedMomentVel)
{
	m_collidedObject = object;
	m_collidedMomentVelocity = collidedMomentVel;
}

void PERObject::SetCurrentPositionToSpawnPosition()
{
	m_objectState->SetSpawnPosition(m_position);
}

bool PERObject::IsHaveToSleep(const PERVec3& playerPos)
{
	return DistanceSquareAandBIgnoringZValue(playerPos, m_position) > m_sleepDistance;
}
