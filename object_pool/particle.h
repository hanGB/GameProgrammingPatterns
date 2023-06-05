#pragma once

class Particle {
public:
	Particle();
	~Particle();

	void Init(double x, double y, double xVel, double yVel, double lifeTime);
	void Update(double time);
	void Render(HDC& memDC, CoordinateData& cd);
	bool InUse() const;

private:
	double m_lifeTimeLeft;
	double m_x, m_y;
	double m_xVel, m_yVel;
	double m_xSize, m_ySize;
};