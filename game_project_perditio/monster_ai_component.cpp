#include "stdafx.h"
#include "monster_ai_component.h"
#include "per_object.h"
#include "black_board.h"

MonsterAiComponent::MonsterAiComponent()
{
	m_AStarCalculator = new AStarCalculator();
}

MonsterAiComponent::~MonsterAiComponent()
{
	delete m_AStarCalculator;
}

void MonsterAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	if (!m_isAStarCalculated) {
		// �÷��̾���� ��� ���
		m_AStarCalculator->SetStartAndDestination(object.GetPosition(), BlackBoard::GetPlayerPos());
		m_AStarCalculator->FindPath();
		m_paths = m_AStarCalculator->GetPaths();
		m_numPath = m_AStarCalculator->GetNumPath();
		m_currentPathIndex = 0;
		m_isAStarCalculated = true;

		// ù ��°�� �� ��ġ������ �� ��° ��ġ���� ��ǥ�ϵ��� ����
		m_currentPathIndex++;
	}

	// �������� ��� �ƹ��͵� ����
	if (m_currentPathIndex >= m_numPath) return;

	// �÷��̾� �̵�
	// ��ġ�� �� ���
	PERVec3 pos = object.GetPosition();
	PERVec3 goal = PERVec3((m_paths[m_currentPathIndex].x - 50) * 0.25, (m_paths[m_currentPathIndex].y - 50) * 0.25, pos.z);

	// �ش� �������� �̵��� ���� ������
	if ((goal.x - c_PATH_GAP < pos.x) && (pos.x < goal.x + c_PATH_GAP)
		&& (goal.y - c_PATH_GAP < pos.y) && (pos.y < goal.y + c_PATH_GAP))
	{
		m_currentPathIndex++;
		object.SetVelocity(PERVec3(0.0, 0.0, 0.0));
		return;
	}

	// �ش� ���� �������� �̵��� ���� �ӵ� ���
	PERVec3 vel = object.GetVelocity();
	double angle = atan2(goal.y - pos.y, goal.x - pos.x);

	vel.x = cos(angle) * c_DEFAULT_VELOCITY;
	vel.y = sin(angle) * c_DEFAULT_VELOCITY;

	object.SetVelocity(vel);
}

void MonsterAiComponent::SetData(PERComponent::AiData data)
{
}
