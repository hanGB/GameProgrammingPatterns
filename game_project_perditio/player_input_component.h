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
	void SwingBlade(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	void ShowObjectName(PERObject& object, PERController& controller, PERAudio& audio, double dTime);

	const int c_DEFAULT_XY_FORCE = 150000;

	// 이동 관련
	int m_XYForce = c_DEFAULT_XY_FORCE;

	// 총알 발사 관련
	double m_shootingCoolTime = 0.0;
	// 검 휘두르기 관련
	double m_swingCoolTime = 0.0;
	// 공격 방향
	int m_dirX = 0;
	int m_dirY = 1;
};
