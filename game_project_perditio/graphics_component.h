#pragma once
#include "per_component.h"

class PERObject;
class PERAudio;
class PERRenderer;

class GraphicsComponent {
public:
	virtual ~GraphicsComponent() {}

	virtual void Update(PERObject& object, PERAudio* audio, double dTime) = 0;
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap) = 0;
	virtual void SetData(PERComponent::GraphicsData data) = 0;
};