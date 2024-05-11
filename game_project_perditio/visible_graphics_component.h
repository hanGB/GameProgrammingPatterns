#pragma once
#include "graphics_component.h"

class VisibleGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	// 오브젝트 사망시 제거용
	virtual void RemoveFloatingUi();

protected:
	PERVec3 m_position;
	PERVec3 m_size;

private:
	// 모양 및 색상 설정
	PERShapeType m_shapeType;
	PERColor m_color;
	// 테두리 설정
	bool m_border;
	int m_borderWidth;
	PERColor m_borderColor;

	PERVec3 m_currentVelocity;
};