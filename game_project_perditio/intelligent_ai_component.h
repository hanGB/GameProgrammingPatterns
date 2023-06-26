#pragma once
#include "per_ai_component.h"

class IntelligentAiComponent : public PERAiComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, double dTime);
	virtual void SetData(PERComponent::AiData data);

	const int c_DEFAULT_VERTICAL_FORCE = 5000;
	const int c_DEFAULT_HORIZONTAL_FORCE = 20000;

	int m_verticalForce = c_DEFAULT_VERTICAL_FORCE;
	int m_horizontalForce = c_DEFAULT_HORIZONTAL_FORCE;
};