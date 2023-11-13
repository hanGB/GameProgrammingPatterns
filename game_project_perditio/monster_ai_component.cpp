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
		= new LeafNode<MonsterAiComponent>("�÷��̾� ã��", &MonsterAiComponent::FindPlayerPosition, *this);
	LeafNode<MonsterAiComponent>* calculatePathNode
		= new LeafNode<MonsterAiComponent>("�� ���", &MonsterAiComponent::CalculatePath, *this);
	LeafNode<MonsterAiComponent>* moveToLastPlayerPositionNode 
		= new LeafNode<MonsterAiComponent>("������ �÷��̾� ��ġ�� �̵�", &MonsterAiComponent::MoveToLastPlayerPosition, *this);
	LeafNode<MonsterAiComponent>* doNothingNode 
		= new LeafNode<MonsterAiComponent>("�ƹ��͵� ���ϱ�", &MonsterAiComponent::DoNothing, *this);

	SequencerNode* MoveToPlayerSequnce = new SequencerNode("�÷��̾�� �̵�");
	MoveToPlayerSequnce->AddChild(findPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(calculatePathNode);
	MoveToPlayerSequnce->AddChild(moveToLastPlayerPositionNode);
	MoveToPlayerSequnce->AddChild(doNothingNode);

	m_behaviorTree = new BehaviorTree("���� �ൿ Ʈ��", MoveToPlayerSequnce);
}

PERBehaviorResult MonsterAiComponent::FindPlayerPosition(PERObject& object, double dTime)
{
	// �����δ� �÷��̾ �� �� �ִ� ��ġ�� �ִ� �� Ȯ���ϴ� ���� �ʿ�
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
	// �������� ��� ��������
	if (m_currentPathIndex >= m_numPath) return PERBehaviorResult::SUCCESS;


	PERVec3 pos = object.GetPosition();
	PERVec3 goal = m_paths[m_currentPathIndex];

	// �ش� �������� �̵��� ���� ������
	if ((goal.x - c_PATH_GAP < pos.x) && (pos.x < goal.x + c_PATH_GAP)
		&& (goal.y - c_PATH_GAP < pos.y) && (pos.y < goal.y + c_PATH_GAP))
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
