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
	virtual void Initialize();

	const double c_DEFAULT_VELOCITY = 2.0;
	const double c_PATH_GAP_2 = 0.05 * 0.05;
	const double c_WANDER_SPEED = 0.5;

private:
	// 행동 트리 생성
	void InitBehaviorTree();

	// 행동 트리 함수
	// 플레이어한테 이동
	PERBehaviorResult FindPlayerPositionAndSetDestination(double dTime);
	PERBehaviorResult CalculatePath(double dTime);
	PERBehaviorResult MoveToDestination(double dTime);
	PERBehaviorResult DoNothing(double dTime);
	// 스폰 위치로 이동
	PERBehaviorResult SetDestinationToSpawnPosition(double dTime);
	// 배회
	PERBehaviorResult MoveToWanderPosition(double dTime);
	PERBehaviorResult SetWanderPosition(double dTime);

	// 행동 트리
	BehaviorTree* m_behaviorTree;

	// A*를 이용한 경로 계산기
	static AStarCalculator* m_AStarCalculator;

	// 도착지
	PERVec3 m_destination;
	// 배회 각도 및 위치
	double m_wanderAngle = 0.0;
	PERVec3 m_wanderPosition;

	// 계산된 경로를 저장 G 이용하기 위한 변수
	bool m_isAStarCalculated = false;
	PERVec3 m_paths[PER_MAX_CELL * PER_MAX_CELL / 10];
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};
