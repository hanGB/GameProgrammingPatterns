#pragma once
#include "graphics_component.h"

class VisibleGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(PERHud& hud, PERAudio& audio, double dTime);
	virtual void Render(PERRenderer& renderer, double frameGap);
	virtual void SetData(PERComponent::GraphicsData data);
	virtual void Initialize(PERComponent::GraphicsData data);

	// ������Ʈ ����� ���ſ�
	virtual void RemoveFloatingUi();

protected:
	PERVec3 m_position;
	PERVec3 m_size;

private:
	// ��� �� ���� ����
	PERShapeType m_shapeType;
	PERColor m_color;
	// �׵θ� ����
	bool m_border;
	int m_borderWidth;
	PERColor m_borderColor;

	PERVec3 m_currentVelocity;
};