#pragma once
#include "input_component.h"
#include "per_controller.h"

class ButtonInputComponent : public InputComponent {
public:
	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize(PERComponent::InputData data);

private:
	const double c_DEFAULT_INPUT_RANGE_SQURE = 1.0;
};
