#include "stdafx.h"
#include "interact_input_component.h"
#include "per_object.h"
#include "per_controller.h"

void InteractInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	Move(controller, audio, dTime);
}

void InteractInputComponent::SetData(PERComponent::InputData data)
{
}

void InteractInputComponent::Initialize(PERComponent::InputData data)
{
	m_XYForce = c_DEFAULT_XY_FORCE;
	SetData(data);
}

void InteractInputComponent::Move(PERController& controller, PERAudio& audio, double dTime)
{
	// 필요 정보 얻기
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	double mass = GetOwner()->GetMass();

	// x, y축 이동 설정
	if (controller.IsKeyboardPressed(KeySetting::MoveUpwards.value)) {
		if (vel.y < GetOwner()->c_MAXIMUM_XY_VELOCITY) {
			cAcc.y += m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveDownwards.value)) {
		if (vel.y > -GetOwner()->c_MAXIMUM_XY_VELOCITY) {
			cAcc.y -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveLeftwards.value)) {
		if (vel.x > -GetOwner()->c_MAXIMUM_XY_VELOCITY) {
			cAcc.x -= m_XYForce / mass * dTime;
		}
	}
	if (controller.IsKeyboardPressed(KeySetting::MoveRightwards.value)) {
		if (vel.x < GetOwner()->c_MAXIMUM_XY_VELOCITY) {
			cAcc.x += m_XYForce / mass * dTime;
		}
	}

	GetOwner()->SetCurrentAccel(cAcc);
}
