#include "stdafx.h"
#include "graphics_component.h"
#include "bjorn.h"
#include "comp_renderer.h"

void GraphicsComponent::Update(Bjorn& bjorn)
{
	CompVector2<double> velocity = bjorn.GetVelocity();

	if (velocity.x < -STOP_VELOCITY) {
		m_color = m_walkLeftColor;
	}
	else if (velocity.x > STOP_VELOCITY) {
		m_color = m_walkRightColor;
	}
	else {
		m_color = m_idleColor;
	}
}

void GraphicsComponent::Render(Bjorn& bjorn, CompRenderer& renderer)
{
	CompVector3<double> pos = bjorn.GetPosition();
	CompVector2<double> size = bjorn.GetSize();

	renderer.RenderShape(m_shapeType, pos, size, m_color);
}

CompShapeType GraphicsComponent::GetShapeType() const
{
	return m_shapeType;
}

void GraphicsComponent::SetShapeType(CompShapeType type)
{
	m_shapeType = type;
}

void GraphicsComponent::SetColors(CompColor idle, CompColor walkLeft, CompColor walkRight)
{
	m_idleColor = idle;
	m_walkLeftColor = walkLeft;
	m_walkRightColor = walkRight;
}
