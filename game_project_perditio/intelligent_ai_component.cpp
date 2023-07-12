#include "stdafx.h"
#include "intelligent_ai_component.h"
#include "per_object.h"

void IntelligentAiComponent::Update(PERObject& object, PERWorld& world, PERAudio* audio, double dTime)
{
	// 필요 정보 얻기
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	cAcc.x -= m_verticalForce / mass * dTime;

	object.SetCurrentAccel(cAcc);
}

void IntelligentAiComponent::SetData(PERComponent::AiData data)
{
}
