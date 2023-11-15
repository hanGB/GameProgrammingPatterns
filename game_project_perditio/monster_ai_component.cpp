#include "stdafx.h"
#include "monster_ai_component.h"
#include "per_object.h"
#include "black_board.h"
#include "monster_state.h"

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
	// 리프 노드 생성
	LeafNode<MonsterAiComponent>* findPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("플레이어 찾기", &MonsterAiComponent::FindPlayerPositionAndSetDestination, *this);
	LeafNode<MonsterAiComponent>* calculatePathNode
		= new LeafNode<MonsterAiComponent>("길 계산", &MonsterAiComponent::CalculatePath, *this);
	LeafNode<MonsterAiComponent>* moveToLastPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("설정된 도착지로 이동", &MonsterAiComponent::MoveToDestination, *this);
	LeafNode<MonsterAiComponent>* doNothingNode 
		= new LeafNode<MonsterAiComponent>("아무것도 안하기", &MonsterAiComponent::DoNothing, *this);
	LeafNode<MonsterAiComponent>* setDestinationToSpawnPositionNode
		= new LeafNode<MonsterAiComponent>("스폰 위치로 도착지 설정", &MonsterAiComponent::SetDestinationToSpawnPosition, *this);
	SequencerNode* MoveToSpawnSequnce = new SequencerNode("스폰 위치 이동");
	LeafNode<MonsterAiComponent>* moveToWanderPositionNode
		= new LeafNode<MonsterAiComponent>("배회 위치로 이동", &MonsterAiComponent::MoveToWanderPosition, *this);
	LeafNode<MonsterAiComponent>* setWanderPositionNode
		= new LeafNode<MonsterAiComponent>("배회 위치 설정", &MonsterAiComponent::SetWanderPosition, *this);
	
	// 플레이어로 이동
	SequencerNode* MoveToPlayerSequnce = new SequencerNode("플레이어로 이동");
	MoveToPlayerSequnce->AddChild(findPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(calculatePathNode);
	MoveToPlayerSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(doNothingNode);

	// 스폰 위치로 이동
	MoveToSpawnSequnce->AddChild(setDestinationToSpawnPositionNode);
	MoveToSpawnSequnce->AddChild(calculatePathNode);
	MoveToSpawnSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToSpawnSequnce->AddChild(setWanderPositionNode);

	// 스폰 위치 주변 배회
	SequencerNode* WanderAroundSpawnPosition = new SequencerNode("스폰 위치 주변 배회");
	WanderAroundSpawnPosition->AddChild(moveToWanderPositionNode);
	WanderAroundSpawnPosition->AddChild(setWanderPositionNode);

	// 플레이어를 못 찾을 경우 스폰 위치에서 배회
	SelectorNode* MoveToPlayerOrWanderAroundSpawnSelector = new SelectorNode("플레이어에게 이동 아니면 스폰 위치 이동 후 배회");
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(MoveToPlayerSequnce);
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(MoveToSpawnSequnce);
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(WanderAroundSpawnPosition);

	m_behaviorTree = new BehaviorTree("몬스터 행동 트리", MoveToPlayerOrWanderAroundSpawnSelector);
}

PERBehaviorResult MonsterAiComponent::FindPlayerPositionAndSetDestination(PERObject& object, double dTime)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();
	MonsterState& state = dynamic_cast<MonsterState&>(object.GetObjectState());

	// 실제로는 플레이어가 볼 수 있는 위치에 있는 지 확인하는 절차 필요
	if (DistanceSquareAandB(object.GetPosition(), playerPos) > state.GetSightSquare()) return PERBehaviorResult::FAIL;

	m_destination = playerPos;
	return PERBehaviorResult::SUCCESS;
}

PERBehaviorResult MonsterAiComponent::CalculatePath(PERObject& object, double dTime)
{
	bool result = m_AStarCalculator->FindPath(object.GetPosition(), m_destination, m_paths, &m_numPath);
	m_currentPathIndex = 0;
	m_currentPathIndex++;

	if (result) return PERBehaviorResult::SUCCESS;
	return PERBehaviorResult::FAIL;
}

PERBehaviorResult MonsterAiComponent::MoveToDestination(PERObject& object, double dTime)
{
	// 도착했을 경우 성공으로
	if (m_currentPathIndex >= m_numPath) return PERBehaviorResult::SUCCESS;


	PERVec3 pos = object.GetPosition(); 
	PERVec3 goal = m_paths[m_currentPathIndex]; goal.z = pos.z;

	// 해당 지점까지 이동시 다음 지점로
	if (DistanceSquareAandB(pos, goal) < c_PATH_GAP_2)
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

PERBehaviorResult MonsterAiComponent::SetDestinationToSpawnPosition(PERObject& object, double dTime)
{
	PERVec3 spawnPos = object.GetObjectState().GetSpawnPosition();
	MonsterState& state = dynamic_cast<MonsterState&>(object.GetObjectState());

	// 배회 거리 보다 스폰 위치까지의 거리가 짧을 경우 이동하지 않음
	if (DistanceSquareAandB(object.GetPosition(), spawnPos) < state.GetWanderDistanceSquare()) return PERBehaviorResult::FAIL;

	m_destination = spawnPos;

	return PERBehaviorResult::SUCCESS;
}

PERBehaviorResult MonsterAiComponent::MoveToWanderPosition(PERObject& object, double dTime)
{
	PERVec3 pos = object.GetPosition();
	PERVec3 goal = m_wanderPosition; goal.z = pos.z;

	// 도착시 성공으로
	if (DistanceSquareAandB(pos, goal) < c_PATH_GAP_2) return PERBehaviorResult::SUCCESS;

	// 해당 지점 방향으로 이동을 위한 속도 계산
	PERVec3 vel = object.GetVelocity();
	double angle = atan2(goal.y - pos.y, goal.x - pos.x);

	vel.x = cos(angle) * c_DEFAULT_VELOCITY;
	vel.y = sin(angle) * c_DEFAULT_VELOCITY;

	object.SetVelocity(vel);

	return PERBehaviorResult::RUNNING;
}

PERBehaviorResult MonsterAiComponent::SetWanderPosition(PERObject& object, double dTime)
{
	m_wanderAngle += dTime * c_WANDER_SPEED;
	
	MonsterState& state = dynamic_cast<MonsterState&>(object.GetObjectState());
	PERVec3 spawnPos = state.GetSpawnPosition();
	double wanderDis = state.GetWanderDistance();
	PERVec3 pos = object.GetPosition();

	m_wanderPosition = PERVec3(spawnPos.x + wanderDis * cos(m_wanderAngle), spawnPos.y + wanderDis * sin(m_wanderAngle), pos.z);

	return PERBehaviorResult::SUCCESS;
}
