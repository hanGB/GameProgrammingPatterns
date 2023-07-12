#pragma once
#include "per_ai_component.h"


class UnintelligentAiComponent : public PERAiComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
};