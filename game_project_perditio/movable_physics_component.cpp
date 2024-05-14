#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"
#include "physics_helper.h"

void MovablePhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	m_MoveFunc(*this, dTime);
	world.CheckCollision(*GetOwner(), dTime);
	//else PERLog::Logger().Info("충돌됨");

	PhysicsComponent::Update(world, audio, dTime);
}

void MovablePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	if (data.friction) m_MoveFunc = &MovablePhysicsComponent::Move;
	else m_MoveFunc = &MovablePhysicsComponent::MoveWithoutFriction;

	PhysicsComponent::SetData(data);
}

void MovablePhysicsComponent::Initialize()
{
	PhysicsComponent::Initialize();
}

void MovablePhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();

	// 충돌한 시간만큼 충돌 속도로 반대로 이동
	pos.x = pos.x - collisionVelocity.x * collisionTime;
	pos.y = pos.y - collisionVelocity.y * collisionTime;

	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(changedVelocity);

	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

void MovablePhysicsComponent::GiveForce(PERWorld& world, PERObject& opponentObject, PERVec3 force, double dTime)
{
	double mass = GetOwner()->GetMass();

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));
	GetOwner()->SetCurrentAccel(PERVec3(force.x / mass, force.y / mass, force.z / mass));

	// 이동 시키고 다시 계산
	m_MoveFunc(*this, dTime);
	world.CheckCollisionWithoutSpecificObject(*GetOwner(), opponentObject, dTime);

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));

	PhysicsComponent::GiveForce(world, opponentObject, force, dTime);
}

void MovablePhysicsComponent::Move(double dTime)
{
	// 필요 정보 얻기
	PERVec3 currentAccel = GetOwner()->GetCurrentAccel();

	// 계산
	PhysicsHelper::CaculateMovementWithFriction(*GetOwner(), currentAccel, dTime);

	// 현재 가속도 초기화
	currentAccel = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(currentAccel);
}

void MovablePhysicsComponent::MoveWithoutFriction(double dTime)
{
	// 필요 정보 얻기
	PERVec3 currentAccel = GetOwner()->GetCurrentAccel();

	// 계산
	PhysicsHelper::CaculateMovementWithoutExternalForce(*GetOwner(), currentAccel, dTime);

	// 현재 가속도 초기화
	currentAccel = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(currentAccel);
}
