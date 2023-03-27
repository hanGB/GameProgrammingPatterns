#pragma once

class Bjorn;
class CompRenderer;
enum class CompShapeType;

class GraphicsComponent {
public:
	void Update(Bjorn& bjorn);
	void Render(Bjorn& bjorn, CompRenderer& renderer);

	CompShapeType GetShapeType() const;
	
	void SetShapeType(CompShapeType type);
	void SetColors(CompColor idle, CompColor walkLeft, CompColor walkRight);
	
private:
	static const int STOP_VELOCITY = 1;

	CompShapeType m_shapeType;
	CompColor m_color;

	CompColor m_idleColor;
	CompColor m_walkRightColor;
	CompColor m_walkLeftColor;
};