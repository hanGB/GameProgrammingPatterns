#pragma once
#include "per_ai_component.h"


class UnintelligentAiComponent : public PERAiComponent {
public:
	virtual void Update(PERObject& object, double dTime);
};