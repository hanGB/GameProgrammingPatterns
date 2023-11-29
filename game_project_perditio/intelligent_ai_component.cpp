#include "stdafx.h"
#include "intelligent_ai_component.h"
#include "per_object.h"
#include "black_board.h"

void IntelligentAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	// ����� ���� �ð�
	object.GetObjectState().UseIgnoreDamageTime(dTime);
	// �ð� �� ȸ��
	object.GetObjectState().RecoverPerTime(object, dTime);

	// �ʿ� ���� ���
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	PERVec3 pos = object.GetPosition();
	double mass = object.GetMass();

	PERVec3 playerPos = BlackBoard::GetPlayerPos();

	double angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	cAcc.x += cos(angle) * m_XYForce / mass * dTime;
	cAcc.y += sin(angle) * m_XYForce / mass * dTime;

	object.SetCurrentAccel(cAcc);
}

void IntelligentAiComponent::SetData(PERComponent::AiData data)
{
}
