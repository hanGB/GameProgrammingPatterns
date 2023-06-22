#pragma once
#include "per_graphics_component.h"

class HiddenGraphicsComponent : public PERGraphicsComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer);
	virtual void SetData(PERComponent::GraphicsData data);
};