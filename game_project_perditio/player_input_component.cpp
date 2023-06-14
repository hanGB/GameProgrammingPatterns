#include "stdafx.h"
#include "player_input_component.h"
#include "per_object.h"
#include "per_controller.h"

void PlayerInputComponent::Update(PERObject& object, PERController& controller)
{
	Move(object, controller);
}

void PlayerInputComponent::Move(PERObject& object, PERController& controller)
{
	// �ʿ� ���� ���
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// �¿� �̵� ����
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_UP)) {
		if (vel.y < object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.y += m_verticalForce / mass;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_DOWN)) {
		if (vel.y > -object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.y -= m_verticalForce / mass;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_LEFT)) {
		if (vel.x > -object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.x -= m_verticalForce / mass;
		}
	}
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_RIGHT)) {
		if (vel.x < object.c_MAXIMUM_VERTICAL_VELOCITY) {
			cAcc.x += m_verticalForce / mass;
		}
	}

	object.SetCurrentAccel(cAcc);
}
