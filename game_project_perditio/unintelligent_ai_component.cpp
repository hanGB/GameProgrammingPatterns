#include "stdafx.h"
#include "unintelligent_ai_component.h"
#include "per_object.h"

void UnintelligentAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{	
	// ����� ���� �ð�
	object.GetObjectState().UseIgnoreDamageTime(dTime);
	// �ð� �� ȸ��
	object.GetObjectState().RecoverPerTime(object, dTime);
}

void UnintelligentAiComponent::SetData(PERComponent::AiData data)
{
}
