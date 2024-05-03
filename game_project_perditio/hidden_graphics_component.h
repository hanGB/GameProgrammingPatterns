#pragma once
#include "graphics_component.h"

class HiddenGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERObject& object, PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize(PERComponent::GraphicsData data);

	virtual void RemoveFloatingUi();
};