#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;

class PERAiComponent {
public:
	virtual ~PERAiComponent() {}

	virtual void Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime) = 0;
	virtual void SetData(PERComponent::AiData data) = 0;
};