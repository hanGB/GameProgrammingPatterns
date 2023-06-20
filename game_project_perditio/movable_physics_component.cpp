#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"

void MovablePhysicsComponent::Update(PERObject& object, PERWorld& world, double dTime)
{
	Move(object, dTime);
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
