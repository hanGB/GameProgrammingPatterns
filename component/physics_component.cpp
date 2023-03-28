#include "stdafx.h"
#include "physics_component.h"
#include "comp_object.h"
#include "comp_world.h"

void BjornPhysicsComponent::Update(CompObject& object, CompWorld& world, double elapsedTimeInSec)
{
	double t = elapsedTimeInSec;

	CompVector3<double> pos;
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	pos = object.GetPosition();
	vel = object.GetVelocity();
	cAcc = object.GetCurrentAccel();

	// 이동 관련
	if (fabs(vel.x) > 0.f && !object.GetIsFalling()) {
		// 마찰
		cAcc.x += vel.x / std::abs(vel.x) * GPP_FRICTION * (-GPP_GRAVITY);

		double tempVelocityX = vel.x + cAcc.x * t;
		if (tempVelocityX * vel.x < 0.0f) {
			vel.x = 0.0f;
		}
		else {
			vel.x = tempVelocityX;
		}

		vel.x += cAcc.x * t;
	}
	else if (object.GetIsFalling()) {
		cAcc.x = 0.0;
		cAcc.y -= GPP_GRAVITY;
		vel.y += cAcc.y * t;
	}
	else {
		vel.x += cAcc.x * t;
	}

	pos.x += (vel.x * t + 0.5f * cAcc.x * t * t);
	pos.y += (vel.y * t + 0.5f * cAcc.y * t * t);

	object.SetIsFalling(!world.CheckCollision(&pos, m_volume));

	object.SetPosition(pos);
	object.SetVelocity(vel);
	object.SetCurrentAccel(cAcc);
}
