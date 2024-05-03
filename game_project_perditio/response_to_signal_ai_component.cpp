#include "stdafx.h"
#include "response_to_signal_ai_component.h"

void ResponeseToSignalAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
}

void ResponeseToSignalAiComponent::SetData(PERComponent::AiData data)
{
}

void ResponeseToSignalAiComponent::Initialize(PERComponent::AiData data)
{
	SetData(data);
}

void ResponeseToSignalAiComponent::Execute(PERObject& object)
{
	m_ExecuteFunc(object);
}

void ResponeseToSignalAiComponent::Revoke(PERObject& object)
{
	m_RevokeFunc(object);
}

void ResponeseToSignalAiComponent::SetExcuteFunc(std::function<void(PERObject&)> func)
{
	m_ExecuteFunc = func;
}

void ResponeseToSignalAiComponent::SetRevokeFunc(std::function<void(PERObject&)> func)
{
	m_RevokeFunc = func;
}
