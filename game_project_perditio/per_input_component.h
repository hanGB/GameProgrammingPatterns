#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERController;

class PERInputComponent {
public:
	virtual ~PERInputComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, double dTime) = 0;
	virtual void SetData(PERComponent::InputData data) = 0;
};