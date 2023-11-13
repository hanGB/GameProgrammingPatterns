#include "stdafx.h"
#include "monster_ai_component.h"
#include "per_object.h"
#include "black_board.h"

AStarCalculator* MonsterAiComponent::m_AStarCalculator = new AStarCalculator();

MonsterAiComponent::MonsterAiComponent()
{
	InitBehaviorTree();
}

MonsterAiComponent::~MonsterAiComponent()
{
	delete m_behaviorTree;
}

void MonsterAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	m_behaviorTree->Run(object, dTime);
}

void MonsterAiComponent::SetData(PERComponent::AiData data)
{
}

void MonsterAiComponent::InitBehaviorTree()
{
	LeafNode<MonsterAiComponent>* findPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("플레이어 찾기", &MonsterAiComponent::FindPlayerPosition, *this);
	LeafNode<MonsterAiComponent>* calculatePathNode
		= new LeafNode<MonsterAiComponent>("길 계산", &MonsterAiComponent::CalculatePath, *this);
	LeafNode<MonsterAiComponent>* moveToLastPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("마지막 플레이어 위치로 이동", &MonsterAiComponent::MoveToLastPlayerPosition, *this);
	LeafNode<MonsterAiComponent>* doNothingNode 
		= new LeafNode<MonsterAiComponent>("아무것도 안하기", &MonsterAiComponent::DoNothing, *this);

	SequencerNode* MoveToPlayerSequnce = new SequencerNode("플레이어로 이동");
	MoveToPlayerSequnce->AddChild(findPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(calculatePathNode);
	MoveToPlayerSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(doNothingNode);

	m_behaviorTree = new BehaviorTree("몬스터 행동 트리", MoveToPlayerSequnce);
}

PERBehaviorResult MonsterAiComponent::FindPlayerPosition(PERObject& object, double dTime)
{
	// 실제로는 플레이어가 볼 수 있는 위치에 있는 지 확인하는 절차 필요
	m_lastPlayerPosition = BlackBoard::GetPlayerPos();

	return PERBehaviorResult::SUCCESS;
}

PERBehaviorResult MonsterAiComponent::CalculatePath(PERObject& object, double dTime)
{
	m_AStarCalculator->FindPath(object.GetPosition(), m_lastPlayerPosition, m_paths, &m_numPath);
	m_currentPathIndex = 0;
	m_currentPathIndex++;

	return PERBehaviorResult::SUCCESS;
}

PERBehaviorResult MonsterAiComponent::MoveToLastPlayerPosition(PERObject& object, double dTime)
{
	// 도착했을 경우 성공으로
	if (m_currentPathIndex >= m_numPath) return PERBehaviorResult::SUCCESS;


	PERVec3 pos = object.GetPosition();
	PERVec3 goal = m_paths[m_currentPathIndex];

	// 해당 지점까지 이동시 다음 지점로
	if ((goal.x - c_PATH_GAP < pos.x) && (pos.x < goal.x + c_PATH_GAP)
		&& (goal.y - c_PATH_GAP < pos.y) && (pos.y < goal.y + c_PATH_GAP))
	{
		m_currentPathIndex++;
		object.SetVelocity(PERVec3(0.0, 0.0, 0.0));
		return PERBehaviorResult::RUNNING;
	}

	// 해당 지점 방향으로 이동을 위한 속도 계산
	PERVec3 vel = object.GetVelocity();
	double angle = atan2(goal.y - pos.y, goal.x - pos.x);

	vel.x = cos(angle) * c_DEFAULT_VELOCITY;
	vel.y = sin(angle) * c_DEFAULT_VELOCITY;

	object.SetVelocity(vel);

	return PERBehaviorResult::RUNNING;
}

PERBehaviorResult MonsterAiComponent::DoNothing(PERObject& object, double dTime)
{
	return PERBehaviorResult::SUCCESS;
}
