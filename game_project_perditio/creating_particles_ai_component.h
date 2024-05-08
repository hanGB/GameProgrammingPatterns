#pragma once
#include "ai_component.h"


class CreatingParticlesAiComponent : public AiComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	void SetParticle(PERShapeType type, PERVec3 size, double mass, PERColor color,
		bool isBorderOn = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

private:
	void CreateParticlesForCircleBombEffect(PERWorld& world, PERAudio& audio, double dTime);

	std::function<void(CreatingParticlesAiComponent&, PERWorld&, PERAudio&, double)> m_CreateParticles;

	double m_particleDelay;
	double m_particleLifeTime;
	double m_time;

	// 소환 파티클 모양
	PERShapeType	m_particleShapeType = PERShapeType::RECTANGLE;
	PERVec3			m_particleSize = PERVec3(0.25, 0.25, 0.25);
	double			m_particleMass = 10;
	PERColor		m_particleColor = PERColor(200, 200, 200);
	bool			m_isParticleBorderOn = true;
	int				m_particleBorderWidth = 1;
	PERColor		m_particleBorderColor = PERColor(0, 0, 0);
};