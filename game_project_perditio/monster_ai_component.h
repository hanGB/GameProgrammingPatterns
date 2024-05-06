#pragma once
#include "ai_component.h"
#include "a_star_calculator.h"
#include "behavior_tree.h"

class MonsterAiComponent : public AiComponent {
public:
	MonsterAiComponent();
	~MonsterAiComponent();

	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	const double c_DEFAULT_VELOCITY = 2.0;
	const double c_PATH_GAP_2 = 0.05 * 0.05;
	const double c_WANDER_SPEED = 0.5;

private:
	// �ൿ Ʈ�� ����
	void InitBehaviorTree();

	// �ൿ Ʈ�� �Լ�
	// �÷��̾����� �̵�
	PERBehaviorResult FindPlayerPositionAndSetDestination(double dTime);
	PERBehaviorResult CalculatePath(double dTime);
	PERBehaviorResult MoveToDestination(double dTime);
	PERBehaviorResult DoNothing(double dTime);
	// ���� ��ġ�� �̵�
	PERBehaviorResult SetDestinationToSpawnPosition(double dTime);
	// ��ȸ
	PERBehaviorResult MoveToWanderPosition(double dTime);
	PERBehaviorResult SetWanderPosition(double dTime);

	// �ൿ Ʈ��
	BehaviorTree* m_behaviorTree;

	// A*�� �̿��� ��� ����
	static AStarCalculator* m_AStarCalculator;

	// ������
	PERVec3 m_destination;
	// ��ȸ ���� �� ��ġ
	double m_wanderAngle = 0.0;
	PERVec3 m_wanderPosition;

	// ���� ��θ� ���� �G �̿��ϱ� ���� ����
	bool m_isAStarCalculated = false;
	PERVec3 m_paths[PER_MAX_CELL * PER_MAX_CELL / 10];
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};
