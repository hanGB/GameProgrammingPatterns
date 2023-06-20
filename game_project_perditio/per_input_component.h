#pragma once
#include "per_component.h"

class PERObject;
class PERController;

class PERInputComponent {
public:
	virtual ~PERInputComponent() {}

	virtual void Update(PERObject& object, PERController& controller) = 0;
	virtual void SetData(PERComponent::InputData data) = 0;
};