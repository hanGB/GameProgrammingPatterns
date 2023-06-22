#include "stdafx.h"
#include "player_input_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_controller.h"

void PlayerInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, double dTime)
{
	Move(object, controller);
	UpdateDirection(object.GetCurrentAccel());
	ShootBullet(object, world, controller, dTime);
}

void PlayerInputComponent::SetData(PERComponent::InputData data)
{
}

void PlayerInputComponent::Move(PERObject& object, PERController& controller)
{
	// 필요 정보 얻기
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	// x, y축 이동 설정
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

void PlayerInputComponent::UpdateDirection(PERVec3 currentAccel)
{
	// 둘 다 가속이 없을 경우 업데이트 안함
	if (currentAccel.x == 0.0 && currentAccel.y == 0.0) return;

	// 방향 설정
	if (currentAccel.x > 0.0) m_dirX = 1;
	else if (currentAccel.x < 0.0) m_dirX = -1;
	else m_dirX = 0;

	if (currentAccel.y > 0.0) m_dirY = 1;
	else if (currentAccel.y < 0.0) m_dirY = -1;
	else m_dirY = 0;
}

void PlayerInputComponent::ShootBullet(PERObject& object, PERWorld& world, PERController& controller, double dTime)
{
	m_shootingCoolTime -= dTime;
	if (m_shootingCoolTime > 0.0) return;

	// 총알 발사
	if (controller.IsKeyboardPressed(PERKeyboardValue::KEYBOARD_D)) {
		PERObject* bullet = world.AddAndGetObject(PERObjectType::OBJECT_TYPE_BULLET);
		m_shootingCoolTime = c_DEFAULT_SHOOT_BULLET_COOL_TIME;

		std::cout << m_dirX << ", " << m_dirY << std::endl;

		PERVec3 speed((double)m_dirX * c_BULLER_VERTICAL_FORCE, (double)m_dirY * c_BULLER_VERTICAL_FORCE, 0.0);
		bullet->SetCurrentAccel(speed);
		bullet->SetParent(&object);
		bullet->SetPosition(PERVec3(object.GetPosition().x, object.GetPosition().y, -1.0));
		bullet->SetLifeTime(3.0f);
	}
}
