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
	AStarCalculator* m_AStarCalculator;

	// ���� ��θ� �̿��ϱ� ���� ����
	bool m_isAStarCalculated = false;
	Cell* m_paths = nullptr;
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};