#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"

void MovablePhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	m_MoveFunc(*this, dTime);
	if (!world.CheckCollision(*GetOwner(), dTime)) GetOwner()->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
	//else PERLog::Logger().Info("충돌됨");
}

void MovablePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	if (data.friction) m_MoveFunc = &MovablePhysicsComponent::Move;
	else m_MoveFunc = &MovablePhysicsComponent::MoveWithoutFriction;
}

void MovablePhysicsComponent::Initialize(PERComponent::PhysicsData data)
{
	SetData(data);
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

	GetOwner()->SetCollidedObject(&collidedObject, collisionVelocity);
}

void MovablePhysicsComponent::GiveForce(PERWorld& world, PERVec3 force, double dTime)
{
	double mass = GetOwner()->GetMass();

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));
	GetOwner()->SetCurrentAccel(PERVec3(force.x / mass, force.y / mass, force.z / mass));

	// 이동 시키고 다시 계산
	m_MoveFunc(*this, dTime);
	if (!world.CheckCollision(*GetOwner(), dTime)) GetOwner()->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));
}

void MovablePhysicsComponent::Move(double dTime)
{
	// 필요 정보 얻기
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	double mass = GetOwner()->GetMass();

	// 마찰력 계산
	if (std::abs(vel.x) > 0.0) {
		cAcc.x += vel.x / std::abs(vel.x) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelX = vel.x + cAcc.x * dTime;

		// 마찰력으로 인해 반대 방향으로 움직이지 않도록 확인
		if (tempVelX * vel.x < 0.0) {
			vel.x = 0.0;
		}
		else {
			vel.x = tempVelX;
		}
	}
	else {
		vel.x = vel.x + cAcc.x * dTime;
	}
	if (std::abs(vel.y) > 0.0) {
		cAcc.y += vel.y / std::abs(vel.y) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelY = vel.y + cAcc.y * dTime;

		// 마찰력으로 인해 반대 방향으로 움직이지 않도록 확인
		if (tempVelY * vel.y < 0.0) {
			vel.y = 0.0;
		}
		else {
			vel.y = tempVelY;
		}
	}
	else {
		vel.y = vel.y + cAcc.y * dTime;
	}

	// 현재 위치 계산
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// 계산 결과 적용
	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(vel);

	// 현재 가속도 초기화
	cAcc = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(cAcc);
}

void MovablePhysicsComponent::MoveWithoutFriction(double dTime)
{
	// 필요 정보 얻기
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	double mass = GetOwner()->GetMass();

	vel.x = vel.x + cAcc.x * dTime;
	vel.y = vel.y + cAcc.y * dTime;

	// 현재 위치 계산
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// 계산 결과 적용
	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(vel);

	// 현재 가속도 초기화
	cAcc = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(cAcc);
}
