#pragma once
#include "per_component.h"

class PERObject;
class PERHud;
class PERAudio;
class PERRenderer;

class GraphicsComponent {
public:
	virtual ~GraphicsComponent();

	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	virtual void RemoveFloatingUi();

	void SetOwner(PERObject* object);
	void SetNextComponent(GraphicsComponent* component);
	void SetPositionZValue(double z);
	PERObject* GetOwner();
	GraphicsComponent* GetNextComponent();
	double GetPositionZValue() const;

private:
	PERObject* m_owner;
	GraphicsComponent* m_nextComponent = nullptr;

	double m_positionZValue = PER_LOWEST_Z_VALUE;
};