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

void InteractInputComponent::Initialize(PERComponent::InputData data)
{
	m_XYForce = c_DEFAULT_XY_FORCE;
	SetData(data);
}

void InteractInputComponent::Move(PERObject& object, PERController& controller, PERAudio& audio, double dTime)
{
	// �ʿ� ���� ���
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// x, y�� �̵� ����
	if (controller.IsKeyboardPressed(KeySetting::MoveUpwards.value)) {
		if (vel.y < object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.y += m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveDownwards.value)) {
		if (vel.y > -object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.y -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveLeftwards.value)) {
		if (vel.x > -object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.x -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveRightwards.value)) {
		if (vel.x < object.c_MAXIMUM_XY_VELOCITY) {
			cAcc.x += m_XYForce / mass * dTime;
		}
	}

	object.SetCurrentAccel(cAcc);
}
