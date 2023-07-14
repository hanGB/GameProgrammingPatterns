#pragma once
#include "graphics_component.h"

class VisibleGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERObject& object, PERAudio& audio, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);

private:
	// 모양 및 색상 설정
	PERShapeType m_shapeType;
	PERColor m_color;
	// 테두리 설정
	bool m_border;
	int m_borderWidth;
	PERColor m_borderColor;

	PERVec3 m_position;
	PERVec3 m_size;
	PERVec3 m_currentVelocity;
};