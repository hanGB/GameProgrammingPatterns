#include "stdafx.h"
#include "making_signal_ai_component.h"
#include "per_object.h"
#include "response_to_signal_ai_component.h"

void MakingSignalAiComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	if (!m_isGetInput) return;
	if (m_isDisposable && m_isOn) return;

	MakeSignal(object, audio, dTime);
}

void MakingSignalAiComponent::SetData(PERComponent::AiData data)
{
	m_isSwitch = data.isSwitch;
	m_isDisposable = data.isDisposable;
}

void MakingSignalAiComponent::SetIsGetInput(bool input)
{
	m_isGetInput = input;
}

void MakingSignalAiComponent::MakeSignal(PERObject& object, PERAudio& audio, double dTime)
{
	PERObject* ativeObejct = object.GetParent();
	if (ativeObejct)
	{
		if (m_isSwitch)
		{
			if (!m_isOn)
			{
				PERLog::Logger().Info("스위치 온");

				// 오브젝트의 ai 컨포넌트에 있는 Execute 함수 실행
				dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Execute(*ativeObejct);
			}
			else
			{
				PERLog::Logger().Info("스위치 오프");
				// 오브젝트의 ai 컨포넌트에 있는 Revoke 함수 실행
				dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Revoke(*ativeObejct);
			}
			m_isOn = (m_isOn + 1) % 2;
		}
		else
		{
			PERLog::Logger().Info("버튼 눌림");
			// 오브젝트의 ai 컨포넌트에 있는 Execute 함수 실행
			dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Execute(*ativeObejct);
		}
	}
	m_isGetInput = false;
}
