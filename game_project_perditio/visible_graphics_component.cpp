#include "stdafx.h"
#include "visible_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"

void VisibleGraphicsComponent::Update(PERObject& object, PERAudio& audio, double dTime)
{
	m_position = object.GetPosition();
	m_size = object.GetSize();
	m_currentVelocity = object.GetVelocity();
}

void VisibleGraphicsComponent::Render(PERObject& object, PERRenderer& renderer, double frameGap)
{	
	PERVec3 renderPos = m_position;
	PERVec3 gap = m_currentVelocity * frameGap * ((double)PER_MICROSEC_PER_UPDATE / 1'000'000.0);
	renderPos = PERVec3(renderPos.x + gap.x, renderPos.y + gap.y, renderPos.z + gap.z);
	renderer.RenderShapeInWorldCoordinate(m_shapeType, renderPos, m_size, m_color, m_border, m_borderWidth, m_borderColor);
}

void VisibleGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	m_shapeType = data.shape;
	m_color = data.color;

	m_border = data.border;
	m_borderWidth = data.borderWidth;
	m_borderColor = data.borderColor;
}
