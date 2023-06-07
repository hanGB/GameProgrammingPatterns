#pragma once

class Particle {
public:
	Particle();
	~Particle();

	void Init(double x, double y, double xVel, double yVel, double lifeTime);
	bool Update(double time);
	void Render(HDC& memDC, CoordinateData& cd);
	bool InUse() const;

	Particle* GetNext() const;
	void SetNext(Particle* next);

private:
	double m_lifeTimeLeft;
	
	union {
		// 사용 중일 때의 상태
		struct {
			double x, y;
			double xVel, yVel;
			double xSize, ySize;
		} live;

		// 사용 중이 아닐 때의 상태
		Particle* next;

	} m_state;
};