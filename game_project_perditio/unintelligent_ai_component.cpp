#include "stdafx.h"
#include "unintelligent_ai_component.h"
#include "per_object.h"

void UnintelligentAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{	
	// 대미지 무시 시간
	object.GetObjectState().UseIgnoreDamageTime(dTime);
	// 시간 당 회복
	object.GetObjectState().RecoverPerTime(object, dTime);
}

void UnintelligentAiComponent::SetData(PERComponent::AiData data)
{
}
