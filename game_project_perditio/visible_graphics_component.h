#pragma once
#include "per_graphics_component.h"

class VisibleGraphicsComponent : public PERGraphicsComponent {
public:
	virtual void Update(PERObject& object, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer);

	void SetShapeType(PERShapeType type);
	void SetColor(PERColor color);

private:
	PERShapeType m_shapeType = PERShapeType::SHAPE_TYPE_RECTANGLE;
	PERColor m_color = PERColor(255, 255, 255);
};