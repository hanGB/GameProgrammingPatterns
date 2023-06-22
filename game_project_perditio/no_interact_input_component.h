#pragma once
#include "per_input_component.h"

class NoInteractInputComponent : public PERInputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, double dTime);
	virtual void SetData(PERComponent::InputData data);
};
