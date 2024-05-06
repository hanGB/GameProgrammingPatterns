#include "stdafx.h"
#include "unintelligent_ai_component.h"
#include "per_object.h"

void UnintelligentAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{	
	// 대미지 무시 시간
	GetOwner()->GetObjectState().UseIgnoreDamageTime(dTime);
	// 시간 당 회복
	GetOwner()->GetObjectState().RecoverPerTime(dTime);
}

void UnintelligentAiComponent::SetData(PERComponent::AiData data)
{
}

void UnintelligentAiComponent::Initialize(PERComponent::AiData data)
{
	SetData(data);
}
