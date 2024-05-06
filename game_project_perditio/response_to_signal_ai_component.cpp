#include "stdafx.h"
#include "response_to_signal_ai_component.h"

void ResponeseToSignalAiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
}

void ResponeseToSignalAiComponent::SetData(PERComponent::AiData data)
{
}

void ResponeseToSignalAiComponent::Initialize(PERComponent::AiData data)
{
	SetData(data);
}

void ResponeseToSignalAiComponent::Execute()
{
	m_ExecuteFunc(this);
}

void ResponeseToSignalAiComponent::Revoke()
{
	m_RevokeFunc(this);
}

void ResponeseToSignalAiComponent::SetExcuteFunc(std::function<void(ResponeseToSignalAiComponent* component)> func)
{
	m_ExecuteFunc = func;
}

void ResponeseToSignalAiComponent::SetRevokeFunc(std::function<void(ResponeseToSignalAiComponent* component)> func)
{
	m_RevokeFunc = func;
}
