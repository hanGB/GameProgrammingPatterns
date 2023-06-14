#pragma once
#include "per_ai_component.h"

class PlayerAiComponent : public PERAiComponent {
public:
	virtual void Update(PERObject& object, double dTime);
};