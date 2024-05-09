#include "stdafx.h"
#include "player_input_component.h"
#include "per_object.h"
#include "per_world.h"
#include "per_controller.h"
#include "event_dispatcher.h"
#include "player_state.h"
#include "black_board.h"
#include "per_database.h"

void PlayerInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	Move(controller, audio, dTime);
	UpdateDirection(GetOwner()->GetCurrentAccel());
	ShootBullet(world, controller, audio, dTime);
	SwingBlade(world, controller, audio, dTime);
	ShowObjectName(controller, audio, dTime);
}

void PlayerInputComponent::SetData(PERComponent::InputData data)
{
}

void PlayerInputComponent::Initialize(PERComponent::InputData data)
{
	m_XYForce = c_DEFAULT_XY_FORCE;
	m_shootingCoolTime = 0.0;
	m_swingCoolTime = 0.0;
	m_dirX = 0;
	m_dirY = 1;

	SetData(data);
}

void PlayerInputComponent::Move(PERController& controller, PERAudio& audio, double dTime)
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

void PlayerInputComponent::ShootBullet(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	m_shootingCoolTime -= dTime;
	if (m_shootingCoolTime > 0.0) return;

	// 총알 발사
	if (controller.IsKeyboardPressed(KeySetting::BulletAttack.value)) {
		if (GetOwner()->GetObjectState().UseMind(10)) {
			PlayerState& state = dynamic_cast<PlayerState&>(GetOwner()->GetObjectState());

			PERVec3 position(GetOwner()->GetPosition());
			PERVec3 speed((double)m_dirX * state.c_BULLET_XY_FORCE, (double)m_dirY * state.c_BULLET_XY_FORCE, 0.0);
			PERStat stat = { 1, 0, 0, 20, 0, 0, 0 };
			world.RequestAddObject(
				GetOwner(), PERObjectType::BULLET, state.GetBulletVisualId().c_str(), PERDatabaseType::VISUAL, 
				stat, position, 3.0, speed);

			m_shootingCoolTime = state.GetShootCoolTime();
		}
	}
}

void PlayerInputComponent::SwingBlade(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	m_swingCoolTime -= dTime;
	if (m_swingCoolTime > 0.0) return;

	// 검 휘두루기
	if (controller.IsKeyboardPressed(KeySetting::BladeAttack.value)) {
		PlayerState& state = dynamic_cast<PlayerState&>(GetOwner()->GetObjectState());

		VisualData* data = world.GetDatabase().GetVisualData(state.GetBladeVisualId().c_str());

		// 플레이어에 대한 상대적 위치를 위치값으로 넘김
		PERVec3 stuckPosition = PERVec3((double)m_dirX * data->size.x * 0.8, (double)m_dirY * data->size.y * 0.8, 0.0);
		PERStat stat = { 1, 0, 0, 20, 0, 0, 0 };
		world.RequestAddObject(
			GetOwner(), PERObjectType::BLADE, state.GetBladeVisualId().c_str(), PERDatabaseType::VISUAL,
			stat, stuckPosition, 0.1);

		m_swingCoolTime = state.GetSwingCoolTime();
	}
}

void PlayerInputComponent::ShowObjectName(PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(KeySetting::ShowInformation.value)) {
		bool showingName = BlackBoard::GetShowingName();

		showingName = (showingName + 1) % 2;

		BlackBoard::SetShowingName(showingName);
	}
}
