#pragma once
#include "per_graphics_component.h"

class PlayerGraphicsComponent : public PERGraphicsComponent {
public:
	virtual ~PlayerGraphicsComponent() {}

	virtual void Update(PERObject& object, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer);

private:
	PERShapeType m_shapeType = PERShapeType::SHAPE_TYPE_ELLIPSE;
	PERColor m_color = PERColor(0, 128, 255);
};