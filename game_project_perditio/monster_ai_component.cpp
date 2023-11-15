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
	// ���� ��� ����
	LeafNode<MonsterAiComponent>* findPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("�÷��̾� ã��", &MonsterAiComponent::FindPlayerPositionAndSetDestination, *this);
	LeafNode<MonsterAiComponent>* calculatePathNode
		= new LeafNode<MonsterAiComponent>("�� ���", &MonsterAiComponent::CalculatePath, *this);
	LeafNode<MonsterAiComponent>* moveToLastPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("������ �������� �̵�", &MonsterAiComponent::MoveToDestination, *this);
	LeafNode<MonsterAiComponent>* doNothingNode 
		= new LeafNode<MonsterAiComponent>("�ƹ��͵� ���ϱ�", &MonsterAiComponent::DoNothing, *this);
	LeafNode<MonsterAiComponent>* setDestinationToSpawnPositionNode
		= new LeafNode<MonsterAiComponent>("���� ��ġ�� ������ ����", &MonsterAiComponent::SetDestinationToSpawnPosition, *this);
	SequencerNode* MoveToSpawnSequnce = new SequencerNode("���� ��ġ �̵�");
	LeafNode<MonsterAiComponent>* moveToWanderPositionNode
		= new LeafNode<MonsterAiComponent>("��ȸ ��ġ�� �̵�", &MonsterAiComponent::MoveToWanderPosition, *this);
	LeafNode<MonsterAiComponent>* setWanderPositionNode
		= new LeafNode<MonsterAiComponent>("��ȸ ��ġ ����", &MonsterAiComponent::SetWanderPosition, *this);
	
	// �÷��̾�� �̵�
	SequencerNode* MoveToPlayerSequnce = new SequencerNode("�÷��̾�� �̵�");
	MoveToPlayerSequnce->AddChild(findPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(calculatePathNode);
	MoveToPlayerSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(doNothingNode);

	// ���� ��ġ�� �̵�
	MoveToSpawnSequnce->AddChild(setDestinationToSpawnPositionNode);
	MoveToSpawnSequnce->AddChild(calculatePathNode);
	MoveToSpawnSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToSpawnSequnce->AddChild(setWanderPositionNode);

	// ���� ��ġ �ֺ� ��ȸ
	SequencerNode* WanderAroundSpawnPosition = new SequencerNode("���� ��ġ �ֺ� ��ȸ");
	WanderAroundSpawnPosition->AddChild(moveToWanderPositionNode);
	WanderAroundSpawnPosition->AddChild(setWanderPositionNode);

	// �÷��̾ �� ã�� ��� ���� ��ġ���� ��ȸ
	SelectorNode* MoveToPlayerOrWanderAroundSpawnSelector = new SelectorNode("�÷��̾�� �̵� �ƴϸ� ���� ��ġ �̵� �� ��ȸ");
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(MoveToPlayerSequnce);
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(MoveToSpawnSequnce);
	MoveToPlayerOrWanderAroundSpawnSelector->AddChild(WanderAroundSpawnPosition);

	m_behaviorTree = new BehaviorTree("���� �ൿ Ʈ��", MoveToPlayerOrWanderAroundSpawnSelector);
}

PERBehaviorResult MonsterAiComponent::FindPlayerPositionAndSetDestination(PERObject& object, double dTime)
{
	PERVec3 playerPos = BlackBoard::GetPlayerPos();
	MonsterState& state = dynamic_cast<MonsterState&>(object.GetObjectState());

	// �����δ� �÷��̾ �� �� �ִ� ��ġ�� �ִ� �� Ȯ���ϴ� ���� �ʿ�
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
	// �������� ��� ��������
	if (m_currentPathIndex >= m_numPath) return PERBehaviorResult::SUCCESS;


	PERVec3 pos = object.GetPosition(); 
	PERVec3 goal = m_paths[m_currentPathIndex]; goal.z = pos.z;

	// �ش� �������� �̵��� ���� ������
	if (DistanceSquareAandB(pos, goal) < c_PATH_GAP_2)
	{
		m_currentPathIndex++;
		object.SetVelocity(PERVec3(0.0, 0.0, 0.0));
		return PERBehaviorResult::RUNNING;
	}

	// �ش� ���� �������� �̵��� ���� �ӵ� ���
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

	// ��ȸ �Ÿ� ���� ���� ��ġ������ �Ÿ��� ª�� ��� �̵����� ����
	if (DistanceSquareAandB(object.GetPosition(), spawnPos) < state.GetWanderDistanceSquare()) return PERBehaviorResult::FAIL;

	m_destination = spawnPos;

	return PERBehaviorResult::SUCCESS;
}

PERBehaviorResult MonsterAiComponent::MoveToWanderPosition(PERObject& object, double dTime)
{
	PERVec3 pos = object.GetPosition();
	PERVec3 goal = m_wanderPosition; goal.z = pos.z;

	// ������ ��������
	if (DistanceSquareAandB(pos, goal) < c_PATH_GAP_2) return PERBehaviorResult::SUCCESS;

	// �ش� ���� �������� �̵��� ���� �ӵ� ���
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
