#include "stdafx.h"
#include "unintelligent_ai_component.h"
#include "per_object.h"

void UnintelligentAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{	
	// ����� ���� �ð�
	GetOwner()->GetObjectState().UseIgnoreDamageTime(dTime);
	// �ð� �� ȸ��
	GetOwner()->GetObjectState().RecoverPerTime(dTime);
}

void UnintelligentAiComponent::SetData(PERComponent::AiData data)
{
}

void UnintelligentAiComponent::Initialize(PERComponent::AiData data)
{
	SetData(data);
}
