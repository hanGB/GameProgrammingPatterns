#pragma once
#include "input_component.h"

class InteractInputComponent : public InputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize(PERComponent::InputData data);

private:
	void Move(PERObject& object, PERController& controller, PERAudio& audio, double dTime);

	// �ʴ� ���ϴ� ��
	const int c_DEFAULT_XY_FORCE = 10000;

	int m_XYForce = c_DEFAULT_XY_FORCE;
};
