#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERController;
class PERAudio;

class InputComponent {
public:
	virtual ~InputComponent() {}

	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime) = 0;
	virtual void SetData(PERComponent::InputData data) = 0;
	virtual void Initialize(PERComponent::InputData data) = 0;

	void SetOwner(PERObject* object)
	{
		m_owner = object;
	}
	PERObject* GetOwner()
	{
		return m_owner;
	}

private:
	PERObject* m_owner;
};