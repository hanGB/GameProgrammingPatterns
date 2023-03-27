#pragma once
#include "comp_renderer.h"
#include "comp_world.h"
#include "input_component.h"

class Bjorn {
public:
	Bjorn();
	~Bjorn();

	void Update(CompWorld& world, double elapsedTimeInSec);
	void Render(CompRenderer& renderer);

	void GetCurrentAcc(double* x, double* y);
	void GetVelocity(double* x, double* y);
	bool GetIsFalling() const;
	double GetMaximumVelocityX() const;
	double GetMass() const;

	void SetCurrentAcc(double x, double y);
	void SetVelocity(double x, double y);
	void SetIsFalling(bool fall);
	void SetMass(double mass);

private:
	void InitCurrentAccel();

	static const int MAXIMUM_VELOCITY_X = 3;
	static const int STOP_VELOCITY = 1;

	InputComponent m_input;

	double m_posX, m_posY;
	double m_sizeX, m_sizeY;
	double m_velocityX, m_velocityY;
	double m_mass;
	double m_currentAccX, m_currentAccY;
	double m_maximumVelocityX = (double)MAXIMUM_VELOCITY_X;

	bool m_isFalling;

	CompRGBColor m_color;

	CompRGBColor m_idleColor;
	CompRGBColor m_walkRightColor;
	CompRGBColor m_walkLeftColor;
};