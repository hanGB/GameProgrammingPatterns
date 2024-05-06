#include "stdafx.h"
#include "intelligent_ai_component.h"
#include "per_object.h"
#include "black_board.h"

void IntelligentAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// ����� ���� �ð�
	GetOwner()->GetObjectState().UseIgnoreDamageTime(dTime);
	// �ð� �� ȸ��
	GetOwner()->GetObjectState().RecoverPerTime(dTime);

	// �ʿ� ���� ���
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	PERVec3 pos = GetOwner()->GetPosition();
	double mass = GetOwner()->GetMass();

	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	double angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	cAcc.x += cos(angle) * m_XYForce / mass * dTime;
	cAcc.y += sin(angle) * m_XYForce / mass * dTime;

	GetOwner()->SetCurrentAccel(cAcc);
}

void IntelligentAiComponent::SetData(PERComponent::AiData data)
{
}

void IntelligentAiComponent::Initialize(PERComponent::AiData data)
{
	m_XYForce = c_DEFAULT_XY_FORCE;
	SetData(data);
}
