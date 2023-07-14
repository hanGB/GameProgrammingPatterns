#include "stdafx.h"
#include "intelligent_ai_component.h"
#include "per_object.h"

void IntelligentAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	// �ʿ� ���� ���
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	if (vel.x < object.c_MAXIMUM_XY_VELOCITY)
		cAcc.x += m_XYForce / mass * dTime;

	object.SetCurrentAccel(cAcc);
}

void IntelligentAiComponent::SetData(PERComponent::AiData data)
{
}
