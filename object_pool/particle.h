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
		// ��� ���� ���� ����
		struct {
			double x, y;
			double xVel, yVel;
			double xSize, ySize;
		} live;

		// ��� ���� �ƴ� ���� ����
		Particle* next;

	} m_state;
};