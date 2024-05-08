#pragma once

class PERRenderer;

class PERParticle {
public:
	PERParticle();
	~PERParticle();

	void Init(PERShapeType type, PERVec3 pos, PERVec3 size, double mass, PERVec3 force, PERVec3 vel, PERColor color,
		double lifeTime = PER_MAXIMUM_LIFE_TIME, bool isBorderOn = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	bool Update(double dTime);
	void Render(PERRenderer& renderer);

	bool GetIsInUse() const;
	void SetIsInUse(bool use);

	PERParticle* GetNext();
	void SetNext(PERParticle* next);

private:
	bool m_isInUse;
	double m_lifeTime;

	// 물리값
	PERVec3 m_position;
	PERVec3 m_size;
	PERVec3 m_force;
	PERVec3 m_velocity;
	double m_mass;

	// 외형
	PERShapeType m_shapeType;
	PERColor m_color;
	bool m_isBorderOn;
	int m_borderWidth;
	PERColor m_borderColor;

	PERParticle* m_next;
};