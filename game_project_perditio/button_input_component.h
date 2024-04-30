#pragma once
#include "input_component.h"

class ButtonInputComponent : public InputComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);

private:
	PERKeyboardValue m_inputKeyValue = PERKeyboardValue::F;			
};
