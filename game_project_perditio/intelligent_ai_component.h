#pragma once
#include "per_ai_component.h"

class IntelligentAiComponent : public PERAiComponent {
public:
	virtual void Update(PERObject& object, double dTime);
	virtual void SetData(PERComponent::AiData data);
};