#include "stdafx.h"
#include "input_component.h"
#include "comp_controller.h"
#include "bjorn.h"

void InputComponent::Update(Bjorn& bjorn)
{
	double velX, velY;
	double cAccX, cAccY;

	bjorn.GetVelocity(&velX, &velY);
	bjorn.GetCurrentAcc(&cAccX, &cAccY);

	if (CompController::GetInstance()->GetXAxisDirection() < 0)
	{
		if (velX > -bjorn.GetMaximumVelocityX()) {
			cAccX -= FORCE_X / bjorn.GetMass();
		}
	}
	else if (CompController::GetInstance()->GetXAxisDirection() > 0)
	{
		if (velX < bjorn.GetMaximumVelocityX()) {
			cAccX += FORCE_X / bjorn.GetMass();
		}
	}

	bjorn.SetVelocity(velX, velY);
	bjorn.SetCurrentAcc(cAccX, cAccY);
}
