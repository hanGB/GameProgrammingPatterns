#include "stdafx.h"
#include "player_input_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_controller.h"
#include "event_dispatcher.h"
#include "player_state.h"

void PlayerInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	Move(object, controller, audio, dTime);
	UpdateDirection(object.GetCurrentAccel());
	ShootBullet(object, world, controller, audio, dTime);
	SwingBlade(object, world, controller, audio, dTime);
}

void PlayerInputComponent::SetData(PERComponent::InputData data)
{
}

void PlayerInputComponent::Move(PERObject& object, PERController& controller, PERAudio& audio, double dTime)
{
	// �ʿ� ���� ���
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// x, y�� �̵� ����
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

void PlayerInputComponent::UpdateDirection(PERVec3 currentAccel)
{
	// �� �� ������ ���� ��� ������Ʈ ����
	if (currentAccel.x == 0.0 && currentAccel.y == 0.0) return;

	// ���� ����
	if (currentAccel.x > 0.0) m_dirX = 1;
	else if (currentAccel.x < 0.0) m_dirX = -1;
	else m_dirX = 0;

	if (currentAccel.y > 0.0) m_dirY = 1;
	else if (currentAccel.y < 0.0) m_dirY = -1;
	else m_dirY = 0;
}

void PlayerInputComponent::ShootBullet(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	m_shootingCoolTime -= dTime;
	if (m_shootingCoolTime > 0.0) return;

	// �Ѿ� �߻�
	if (controller.IsKeyboardPressed(PERKeyboardValue::D)) {
		if (object.GetObjectState().UseMind(object, 10)) {
			PlayerState& state = dynamic_cast<PlayerState&>(object.GetObjectState());

			PERVec3 position(object.GetPosition().x, object.GetPosition().y, PER_NORAML_OBJECT_Z_VALUE);
			PERVec3 speed((double)m_dirX * state.c_BULLET_XY_FORCE, (double)m_dirY * state.c_BULLET_XY_FORCE, 0.0);
			PERStat stat = { 0, 0, 20, 0, 0, 0 };
			world.RequestAddObject(
				&object, PERObjectType::BULLET,
				position, state.GetBulletSize(), speed, stat, 3.0);

			m_shootingCoolTime = state.GetShootCoolTime();
		}
	}
}

void PlayerInputComponent::SwingBlade(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	m_swingCoolTime -= dTime;
	if (m_swingCoolTime > 0.0) return;

	// �� �ֵη��
	if (controller.IsKeyboardPressed(PERKeyboardValue::S)) {
		PlayerState& state = dynamic_cast<PlayerState&>(object.GetObjectState());

		// �÷��̾ ���� ����� ��ġ�� ��ġ������ �ѱ�
		PERVec3 stuckPosition = PERVec3((double)m_dirX * state.GetBladeSize().x, (double)m_dirY * state.GetBladeSize().y, 0.0);
		PERStat stat = { 0, 0, 10, 0, 0, 0 };
		world.RequestAddObject(
			&object, PERObjectType::BLADE,
			stuckPosition, state.GetBladeSize(), PERVec3(0.0, 0.0, 0.0), stat, 0.3);

		m_swingCoolTime = state.GetSwingCoolTime();
	}
}
