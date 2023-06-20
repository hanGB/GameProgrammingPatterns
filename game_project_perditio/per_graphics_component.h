#pragma once
#include "per_component.h"

class PERObject;
class PERRenderer;

class PERGraphicsComponent {
public:
	virtual ~PERGraphicsComponent() {}

	virtual void Update(PERObject& object, double dTime) = 0;
	virtual void Render(PERObject& object, PERRenderer& renderer) = 0;
	virtual void SetData(PERComponent::GraphicsData data) = 0;
};