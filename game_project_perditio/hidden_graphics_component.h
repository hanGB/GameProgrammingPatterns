#pragma once
#include "per_graphics_component.h"

class HiddenGraphicsComponent : public PERGraphicsComponent {
public:
	virtual void Update(PERObject& object, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
};