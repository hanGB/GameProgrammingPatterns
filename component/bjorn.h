#pragma once
#include "comp_renderer.h"
#include "comp_world.h"

class Bjorn {
public:
	Bjorn();
	~Bjorn();

	void Update(CompWorld& world, double elapsedTimeInSec);
	void Render(CompRenderer& renderer);

private:
	static const int MAXIUM_VELOCITY_X = 3;
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;
	static const int STOP_VELOCITY = 1;

	double m_posX, m_posY;
	double m_sizeX, m_sizeY;
	double m_velocityX, m_velocityY;
	double m_mass;

	bool m_isFalling;

	CompRGBColor m_color;

	CompRGBColor m_idleColor;
	CompRGBColor m_walkRightColor;
	CompRGBColor m_walkLeftColor;
};