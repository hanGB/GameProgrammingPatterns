#include "stdafx.h"
#include "physics_component.h"
#include "bjorn.h"
#include "comp_world.h"

void PhysicsComponent::Update(Bjorn& bjorn, CompWorld& world, double elapsedTimeInSec)
{
	double t = elapsedTimeInSec;

	CompVector3<double> pos;
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	pos = bjorn.GetPosition();
	vel = bjorn.GetVelocity();
	cAcc = bjorn.GetCurrentAccel();

	// 이동 관련
	if (fabs(vel.x) > 0.f && !bjorn.GetIsFalling()) {
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
	else if (bjorn.GetIsFalling()) {
		cAcc.x = 0.0;
		cAcc.y -= GPP_GRAVITY;
		vel.y += cAcc.y * t;
	}
	else {
		vel.x += cAcc.x * t;
	}

	pos.x += (vel.x * t + 0.5f * cAcc.x * t * t);
	pos.y += (vel.y * t + 0.5f * cAcc.y * t * t);

	bjorn.SetIsFalling(!world.CheckCollision(&pos, m_volume));

	bjorn.SetPosition(pos);
	bjorn.SetVelocity(vel);
	bjorn.SetCurrentAccel(cAcc);
}

CompVector2<double> PhysicsComponent::GetVolume() const
{
	return m_volume;
}

void PhysicsComponent::SetVolume(CompVector2<double> volume)
{
	m_volume = volume;
}
