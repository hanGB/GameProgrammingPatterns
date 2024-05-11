#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERController;
class PERAudio;

class InputComponent {
public:
	virtual ~InputComponent();

	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize();

	void SetOwner(PERObject* object);
	void SetNextComponent(InputComponent* component);
	PERObject* GetOwner();
	InputComponent* GetNextComponent();

private:
	PERObject* m_owner;
	InputComponent* m_nextComponent = nullptr;
};