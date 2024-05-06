#pragma once
#include "ai_component.h"


class ResponeseToSignalAiComponent : public AiComponent {
public:
	virtual void Update(PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	void Execute();
	void Revoke();

	void SetExcuteFunc(std::function<void(ResponeseToSignalAiComponent* component)> func);
	void SetRevokeFunc(std::function<void(ResponeseToSignalAiComponent* component)> func);

private:
	std::function<void(ResponeseToSignalAiComponent* component)> m_ExecuteFunc;
	std::function<void(ResponeseToSignalAiComponent* component)> m_RevokeFunc;
};