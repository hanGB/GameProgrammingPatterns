#pragma once
#include "per_component.h"

class PERObject;

class PERAiComponent {
public:
	virtual ~PERAiComponent() {}

	virtual void Update(PERObject& object, double dTime) = 0;
	virtual void SetData(PERComponent::AiData data) = 0;
};