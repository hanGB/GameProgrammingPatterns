#include "stdafx.h"
#include "input_component.h"
#include "comp_controller.h"
#include "bjorn.h"

void PlayerInputComponent::Update(Bjorn& bjorn)
{
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	vel = bjorn.GetVelocity();
	cAcc = bjorn.GetCurrentAccel();

	if (CompController::GetInstance()->GetXAxisDirection() < 0)
	{
		if (vel.x > -bjorn.GetMaximumVelocityX()) {
			cAcc.x -= FORCE_X / bjorn.GetMass();
		}
	}
	else if (CompController::GetInstance()->GetXAxisDirection() > 0)
	{
		if (vel.x < bjorn.GetMaximumVelocityX()) {
			cAcc.x += FORCE_X / bjorn.GetMass();
		}
	}

	bjorn.SetVelocity(vel);
	bjorn.SetCurrentAccel(cAcc);
}

void DemoInputComponent::Update(Bjorn& bjorn)
{
	CompVector3<double> pos;
	CompVector2<double> vel;
	CompVector2<double> cAcc;

	pos = bjorn.GetPosition();
	vel = bjorn.GetVelocity();
	cAcc = bjorn.GetCurrentAccel();

	if (pos.x < -2.0) {
		m_direction = 1;
	}
	else if (pos.x > 2.0) {
		m_direction = -1;
	}

	if (abs(vel.x) < bjorn.GetMaximumVelocityX()) {
		cAcc.x += m_direction * FORCE_X / bjorn.GetMass();
	}

	bjorn.SetVelocity(vel);
	bjorn.SetCurrentAccel(cAcc);
}
