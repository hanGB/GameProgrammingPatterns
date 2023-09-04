#pragma once
#include "graphics_component.h"

class VisibleGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERObject& object, PERAudio& audio, double dTime);
	virtual void Render(PERObject& object, PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);

private:
	void RenderWithFloatingUi(PERObject& object, PERRenderer& renderer, double frameGap);
	void RenderOnlyObject(PERObject& object, PERRenderer& renderer, double frameGap);

	// ��� �� ���� ����
	PERShapeType m_shapeType;
	PERColor m_color;
	// �׵θ� ����
	bool m_border;
	int m_borderWidth;
	PERColor m_borderColor;

	PERVec3 m_position;
	PERVec3 m_size;
	PERVec3 m_currentVelocity;

	std::function<void(VisibleGraphicsComponent&, PERObject&, PERRenderer&, double)> m_RenderFunc;
};