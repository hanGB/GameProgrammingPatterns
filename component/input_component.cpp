#include "stdafx.h"
#include "input_component.h"
#include "comp_controller.h"
#include "bjorn.h"

void InputComponent::Update(Bjorn& bjorn)
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
