#pragma once
#include "comp_renderer.h"

class CompObject;

class GraphicsComponent {
public:
	virtual ~GraphicsComponent() {}
	virtual void Update(CompObject& object) = 0;
	virtual void Render(CompObject& object, CompRenderer& renderer) = 0;
};

class BjornGraphicsComponent : public GraphicsComponent {
public:
	virtual void Update(CompObject& object);
	virtual void Render(CompObject& object, CompRenderer& renderer);
	
private:
	static const int STOP_VELOCITY = 1;

	CompShapeType m_shapeType = CompShapeType::COMP_SHAPE_TYPE_ELLIPSE;
	CompColor m_color;

	CompColor m_idleColor = { 0, 127, 127 };
	CompColor m_walkRightColor = { 0, 127, 0 };
	CompColor m_walkLeftColor = { 0, 0, 127 };
};