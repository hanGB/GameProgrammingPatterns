#pragma once
#include "per_input_component.h"

class InteractInputComponent : public PERInputComponent {
public:
	virtual void Update(PERObject& object, PERController& controller);

};
