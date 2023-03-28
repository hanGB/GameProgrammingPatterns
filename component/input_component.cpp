#include "stdafx.h"
#include "input_component.h"
#include "comp_controller.h"
#include "comp_object.h"

void PlayerInputComponent::Update(CompObject& object)
{
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	vel = object.GetVelocity();
	cAcc = object.GetCurrentAccel();

	if (CompController::GetInstance()->GetXAxisDirection() < 0)
	{
		if (vel.x > -object.GetMaximumVelocityX()) {
			cAcc.x -= FORCE_X / object.GetMass();
		}
	}
	else if (CompController::GetInstance()->GetXAxisDirection() > 0)
	{
		if (vel.x < object.GetMaximumVelocityX()) {
			cAcc.x += FORCE_X / object.GetMass();
		}
	}

	object.SetVelocity(vel);
	object.SetCurrentAccel(cAcc);
}

void DemoInputComponent::Update(CompObject& object)
{
	CompVector3<double> pos;
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	pos = object.GetPosition();
	vel = object.GetVelocity();
	cAcc = object.GetCurrentAccel();

	if (pos.x < -2.0) {
		m_direction = 1;
	}
	else if (pos.x > 2.0) {
		m_direction = -1;
	}

	if (abs(vel.x) < object.GetMaximumVelocityX()) {
		cAcc.x += m_direction * FORCE_X / object.GetMass();
	}

	object.SetVelocity(vel);
	object.SetCurrentAccel(cAcc);
}
