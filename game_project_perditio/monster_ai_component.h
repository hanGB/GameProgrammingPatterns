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
	// 행동 트리 생성
	void InitBehaviorTree();

	// 행동 트리 함수
	PERBehaviorResult FindPlayerPosition(PERObject& object, double dTime);
	PERBehaviorResult CalculatePath(PERObject& object, double dTime);
	PERBehaviorResult MoveToLastPlayerPosition(PERObject& object, double dTime);
	PERBehaviorResult DoNothing(PERObject& object, double dTime);

	// 행동 트리
	BehaviorTree* m_behaviorTree;

	// A*를 이용한 경로 계산기
	static AStarCalculator* m_AStarCalculator;

	// 마지막으로 발견된 플레이어 위치
	PERVec3 m_lastPlayerPosition;

	// 계산된 경로를 저장 밎 이용하기 위한 변수
	bool m_isAStarCalculated = false;
	PERVec3 m_paths[PER_MAX_CELL * PER_MAX_CELL / 10];
	int m_numPath = 0;
	int m_currentPathIndex = 0;
};
