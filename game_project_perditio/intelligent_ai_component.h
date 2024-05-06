#pragma once
#include "ai_component.h"

class IntelligentAiComponent : public AiComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	const int c_DEFAULT_XY_FORCE = 30000;

	int m_XYForce = c_DEFAULT_XY_FORCE;
};