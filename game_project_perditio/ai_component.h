#pragma once
#include "per_component.h"

class PERObject;
class PERWorld;
class PERAudio;
class PERObject;

class AiComponent {
public:
	virtual ~AiComponent() {}

	virtual void Update(PERWorld& world, PERAudio& audio, double dTime) = 0;
	virtual void SetData(PERComponent::AiData data) = 0;
	virtual void Initialize(PERComponent::AiData data) = 0;

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