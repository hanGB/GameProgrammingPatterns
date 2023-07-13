#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"

void MovablePhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime)
{
	m_MoveFunc(*this, object, dTime);
	if (world.CheckCollision(object, object.GetPosition(), object.GetSize(), object.GetVelocity(), object.GetBoundingType())) {
		// 처리
	}
}

void MovablePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	if (data.friction) m_MoveFunc = &MovablePhysicsComponent::Move;
	else m_MoveFunc = &MovablePhysicsComponent::MoveWithoutFriction;
}

void MovablePhysicsComponent::ProcessCollision(PERObject& myObject, 
	PERVec3 otherPos, PERVec3 otherSize, PERVec3 otherVel, PERBoundingType otherType)
{
	PERVec3 myPos = myObject.GetPosition(), myHalfSize = myObject.GetSize() * 0.5, myVel = myObject.GetVelocity();
	PERBoundingType myType = myObject.GetBoundingType();
	PERVec3 otherHalfSize = otherSize * 0.5;
	double moveGap = 0.1;

	if (myType == PERBoundingType::RECTANGLE && otherType == PERBoundingType::RECTANGLE) {
		double myLeft = myPos.x - myHalfSize.x, myRight = myPos.x + myHalfSize.x,
			myBottom = myPos.y - myHalfSize.y, myTop = myPos.y + myHalfSize.y;
		double otherLeft = otherPos.x - otherHalfSize.x, otherRight = otherPos.x + otherHalfSize.x,
			otherBottom = otherPos.y - otherHalfSize.y, otherTop = otherPos.y + otherHalfSize.y;


		if (otherVel.x == 0.0 && otherVel.y == 0.0) {
			if (otherPos.x < myLeft && myLeft < otherRight && myVel.x < 0.0) {
				myPos.x = myPos.x + (otherRight - myLeft);
			}
			if (otherLeft < myRight && myRight < otherPos.x && myVel.x > 0.0) {
				myPos.x = myPos.x - (myRight - otherLeft);
			}
			if (otherPos.y < myBottom && myBottom < otherTop && myVel.y < 0.0) {
				myPos.y = myPos.y + (otherTop - myBottom);
			}
			if (otherBottom < myTop && myTop < otherPos.y && myVel.y > 0.0) {
				myPos.y = myPos.y - (myTop - otherBottom);
			}
		}
		myObject.SetPosition(myPos);
	}
}

void MovablePhysicsComponent::Move(PERObject& object, double dTime)
{
	// 필요 정보 얻기
	PERVec3 pos = object.GetPosition();
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

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
	object.SetPosition(pos);
	object.SetVelocity(vel);

	// 현재 가속도 초기화
	cAcc = PERVec3(0.0, 0.0, 0.0);
	object.SetCurrentAccel(cAcc);
}

void MovablePhysicsComponent::MoveWithoutFriction(PERObject& object, double dTime)
{
	// 필요 정보 얻기
	PERVec3 pos = object.GetPosition();
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	vel.x = vel.x + cAcc.x * dTime;
	vel.y = vel.y + cAcc.y * dTime;

	// 현재 위치 계산
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// 계산 결과 적용
	object.SetPosition(pos);
	object.SetVelocity(vel);

	// 현재 가속도 초기화
	cAcc = PERVec3(0.0, 0.0, 0.0);
	object.SetCurrentAccel(cAcc);
}
