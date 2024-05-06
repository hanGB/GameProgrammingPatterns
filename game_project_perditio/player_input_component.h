#pragma once
#include "input_component.h"

class PlayerInputComponent : public InputComponent {
public:
	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize(PERComponent::InputData data);

private:
	void Move(PERController& controller, PERAudio& audio, double dTime);
	void UpdateDirection(PERVec3 currentAccel);
	void ShootBullet(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	void SwingBlade(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	void ShowObjectName(PERController& controller, PERAudio& audio, double dTime);

	const int c_DEFAULT_XY_FORCE = 150000;

	// �̵� ����
	int m_XYForce = c_DEFAULT_XY_FORCE;

	// �Ѿ� �߻� ����
	double m_shootingCoolTime = 0.0;
	// �� �ֵθ��� ����
	double m_swingCoolTime = 0.0;
	// ���� ����
	int m_dirX = 0;
	int m_dirY = 1;
};
