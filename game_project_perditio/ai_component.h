#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;
class PERObject;

class AiComponent {
public:
	virtual ~AiComponent();

	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize();

	void SetOwner(PERObject* object);
	void SetNextComponent(AiComponent* component);
	PERObject* GetOwner();
	AiComponent* GetNextComponent();

private:
	PERObject* m_owner;
	AiComponent* m_nextComponent = nullptr;
};