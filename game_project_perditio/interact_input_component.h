#pragma once
#include "input_component.h"

class InteractInputComponent : public InputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio* audio, double dTime);
	virtual void SetData(PERComponent::InputData data);

private:
	void Move(PERObject& object, PERController& controller, PERAudio* audio, double dTime);

	// 초당 가하는 힘
	const int c_DEFAULT_VERTICAL_FORCE = 10000;
	const int c_DEFAULT_HORIZONTAL_FORCE = 20000;

	int m_verticalForce = c_DEFAULT_VERTICAL_FORCE;
	int m_horizontalForce = c_DEFAULT_HORIZONTAL_FORCE;
};
