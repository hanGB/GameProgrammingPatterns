#pragma once
#include "per_graphics_component.h"

class VisibleGraphicsComponent : public PERGraphicsComponent {
public:
	virtual void Update(PERObject& object, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer);
	virtual void SetData(PERComponent::GraphicsData data);

private:
	PERShapeType m_shapeType = PERShapeType::SHAPE_TYPE_RECTANGLE;
	PERColor m_color = PERColor(255, 255, 255);

	PERVec3 m_position;
	PERVec3 m_size;
};