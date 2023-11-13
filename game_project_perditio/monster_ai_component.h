#pragma once
#include "ai_component.h"
#include "a_star_calculator.h"
#include "behavior_tree.h"

class MonsterAiComponent : public AiComponent {
public:
	MonsterAiComponent();
	~MonsterAiComponent();

	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);

	const double c_DEFAULT_VELOCITY = 2.0;
	const double c_PATH_GAP = 0.05;

private:
	// �ൿ Ʈ�� ����
	void InitBehaviorTree();

	// �ൿ Ʈ�� �Լ�
	PERBehaviorResult FindPlayerPosition(PERObject& object, double dTime);
	PERBehaviorResult CalculatePath(PERObject& object, double dTime);
	PERBehaviorResult MoveToLastPlayerPosition(PERObject& object, double dTime);
	PERBehaviorResult DoNothing(PERObject& object, double dTime);

	// �ൿ Ʈ��
	BehaviorTree* m_behaviorTree;

	// A*�� �̿��� ��� ����
	static AStarCalculator* m_AStarCalculator;

	// ���������� �߰ߵ� �÷��̾� ��ġ
	PERVec3 m_lastPlayerPosition;

	// ���� ��θ� ���� �G �̿��ϱ� ���� ����
	bool m_isAStarCalculated = false;
	PERVec3 m_paths[PER_MAX_CELL * PER_MAX_CELL / 10];
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};
