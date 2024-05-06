#pragma once
#include "per_component.h"

class PERObject;
class PERHud;
class PERAudio;
class PERRenderer;

class GraphicsComponent {
public:
	virtual ~GraphicsComponent() {}

	virtual void Update(PERHud& hud, PERAudio& audio, double dTime) = 0;
	virtual void Render(PERRenderer& renderer, double frameGap) = 0;
	virtual void SetData(PERComponent::GraphicsData data) = 0;
	virtual void Initialize(PERComponent::GraphicsData data) = 0;

	virtual void RemoveFloatingUi() = 0;

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