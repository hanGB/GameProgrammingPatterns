#pragma once
#include "input_component.h"

class NoInteractInputComponent : public InputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize(PERComponent::InputData data);
};
