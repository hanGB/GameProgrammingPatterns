#include "stdafx.h"
#include "graphics_component.h"
#include "comp_object.h"

void BjornGraphicsComponent::Update(CompObject& object)
{
	CompVector2<double> velocity = object.GetVelocity();

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

void BjornGraphicsComponent::Render(CompObject& object, CompRenderer& renderer)
{
	CompVector3<double> pos = object.GetPosition();
	CompVector2<double> size = object.GetSize();

	renderer.RenderShape(m_shapeType, pos, size, m_color);
}