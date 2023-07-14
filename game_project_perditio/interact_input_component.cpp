#include "stdafx.h"
#include "interact_input_component.h"
#include "per_object.h"
#include "per_controller.h"

void InteractInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	Move(object, controller, audio, dTime);
}

void InteractInputComponent::SetData(PERComponent::InputData data)
{
}

void InteractInputComponent::Move(PERObject& object, PERController& controller, PERAudio& audio, double dTime)
{
	// 필요 정보 얻기
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// x, y축 이동 설정
	if (controller.IsKeyboardPressed(PERKeyboardValue::UP)) {
		if (vel.y < object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.y += m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::DOWN)) {
		if (vel.y > -object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.y -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::LEFT)) {
		if (vel.x > -object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.x -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::RIGHT)) {
		if (vel.x < object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.x += m_XYForce / mass * dTime;
		}
	}

	object.SetCurrentAccel(cAcc);
}
