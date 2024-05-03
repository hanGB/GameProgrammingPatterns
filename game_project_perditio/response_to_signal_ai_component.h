#pragma once
#include "ai_component.h"


class ResponeseToSignalAiComponent : public AiComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	void Execute(PERObject& object);
	void Revoke(PERObject& object);

	void SetExcuteFunc(std::function<void(PERObject&)> func);
	void SetRevokeFunc(std::function<void(PERObject&)> func);

private:
	std::function<void(PERObject&)> m_ExecuteFunc;
	std::function<void(PERObject&)> m_RevokeFunc;
};