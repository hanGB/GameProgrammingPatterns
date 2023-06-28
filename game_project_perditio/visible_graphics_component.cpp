#include "stdafx.h"
#include "visible_graphics_component.h"
#include "per_object.h"
#include "per_renderer.h"

void VisibleGraphicsComponent::Update(PERObject& object, double dTime)
{
	m_position = object.GetPosition();
	m_size = object.GetSize();
}

void VisibleGraphicsComponent::Render(PERObject& object, PERRenderer& renderer)
{
	renderer.RenderShapeInWorldCoordinate(m_shapeType, m_position, m_size, m_color, m_border, m_borderWidth, m_borderColor);
}

void VisibleGraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	m_shapeType = data.shape;
	m_color = data.color;

	m_border = data.border;
	m_borderWidth = data.borderWidth;
	m_borderColor = data.borderColor;
}

