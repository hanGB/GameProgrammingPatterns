#pragma once
#include "input_component.h"

class PlayerInputComponent : public InputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);

private:
	void Move(PERObject& object, PERController& controller, PERAudio& audio, double dTime);
	void UpdateDirection(PERVec3 currentAccel);
	void ShootBullet(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);

	const int c_DEFAULT_XY_FORCE = 100000;

	const int c_BULLER_XY_FORCE = 1000;
	const double c_DEFAULT_SHOOT_BULLET_COOL_TIME = 0.5;

	// 이동 관련
	int m_XYForce = c_DEFAULT_XY_FORCE;

	// 총알 발사 관련
	double m_shootingCoolTime = 0.0;
	int m_dirX = 0;
	int m_dirY = 1;
};
