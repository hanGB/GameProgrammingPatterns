#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERController;
class PERAudio;

class PERInputComponent {
public:
	virtual ~PERInputComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio* audio, double dTime) = 0;
	virtual void SetData(PERComponent::InputData data) = 0;
};