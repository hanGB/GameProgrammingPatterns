#pragma once
#include "ai_component.h"


class UnintelligentAiComponent : public AiComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize();
};