#pragma once
#include "per_input_component.h"

class InteractInputComponent : public PERInputComponent {
public:
	virtual void Update(PERObject& object, PERController& controller);
	virtual void SetData(PERComponent::InputData data);

private:
	void Move(PERObject& object, PERController& controller);

	const int c_DEFAULT_VERTICAL_FORCE = 1000;
	const int c_DEFAULT_HORIZONTAL_FORCE = 2000;

	int m_verticalForce = c_DEFAULT_VERTICAL_FORCE;
	int m_horizontalForce = c_DEFAULT_HORIZONTAL_FORCE;
};
