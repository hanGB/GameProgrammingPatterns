#include "stdafx.h"
#include "monster_ai_component.h"
#include "per_object.h"
#include "black_board.h"

AStarCalculator* MonsterAiComponent::m_AStarCalculator = new AStarCalculator();

MonsterAiComponent::MonsterAiComponent()
{
	
}

MonsterAiComponent::~MonsterAiComponent()
{
	
}

void MonsterAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	if (!m_isAStarCalculated) {
		// �÷��̾���� ��� ���
		m_AStarCalculator->FindPath(object.GetPosition(), BlackBoard::GetPlayerPos(), m_paths, &m_numPath);
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
	PERVec3 goal = m_paths[m_currentPathIndex];

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
