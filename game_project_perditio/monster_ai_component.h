#pragma once
#include "ai_component.h"
#include "a_star_calculator.h"

class MonsterAiComponent : public AiComponent {
public:
	MonsterAiComponent();
	~MonsterAiComponent();

	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);

	const double c_DEFAULT_VELOCITY = 2.0;
	const double c_PATH_GAP = 0.05;

private:
	// A*�� �̿��� ��� ����
	static AStarCalculator* m_AStarCalculator;

	// ���� ��θ� ���� �G �̿��ϱ� ���� ����
	bool m_isAStarCalculated = false;
	PERVec3 m_paths[PER_MAX_CELL * PER_MAX_CELL / 10];
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};