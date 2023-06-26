#include "stdafx.h"
#include "interact_input_component.h"
#include "per_object.h"
#include "per_controller.h"

void InteractInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, double dTime)
{
	Move(object, controller, dTime);
}

void InteractInputComponent::SetData(PERComponent::InputData data)
{
}

void InteractInputComponent::Move(PERObject& object, PERController& controller, double dTime)
{
	// 필요 정보 얻기
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// x, y축 이동 설정
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_UP)) {
		if (vel.y < object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.y += m_verticalForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN)) {
		if (vel.y > -object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.y -= m_verticalForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT)) {
		if (vel.x > -object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.x -= m_verticalForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT)) {
		if (vel.x < object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.x += m_verticalForce / mass * dTime;
		}
	}

	object.SetCurrentAccel(cAcc);
}
