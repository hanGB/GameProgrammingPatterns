#pragma once
#include "per_component.h"

class PERObject;
class PERAudio;
class PERRenderer;

class PERGraphicsComponent {
public:
	virtual ~PERGraphicsComponent() {}

	virtual void Update(PERObject& object, PERAudio* audio, double dTime) = 0;
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap) = 0;
	virtual void SetData(PERComponent::GraphicsData data) = 0;
};