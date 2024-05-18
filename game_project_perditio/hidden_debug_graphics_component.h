#pragma once
#include "graphics_component.h"

class HiddenDebugGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize();

	void SetDebug(bool debug);

protected:
	PERVec3 m_position;
	PERVec3 m_size;

private:
	bool m_isDebugOn = true;

	// 모양 및 색상 설정
	PERShapeType m_shapeType = PERShapeType::RECTANGLE;
	PERColor m_color = PERColor(255, 255, 255);
	// 테두리 설정
	bool m_border = true;
	int m_borderWidth = 1;
	PERColor m_borderColor = PERColor(100, 255, 100);
};