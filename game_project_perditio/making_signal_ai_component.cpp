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

void MakingSignalAiComponent::Initialize(PERComponent::AiData data)
{
	m_isGetInput = false;
	m_isSwitch = false;
	m_isOn = false;
	m_isDisposable = false;
	SetData(data);
}

void MakingSignalAiComponent::SetIsGetInput(bool input)
{
	m_isGetInput = input;
}

void MakingSignalAiComponent::SetOnOffGraphicsData(PERComponent::GraphicsData onGraphicsData, PERComponent::GraphicsData offGraphicsData)
{
	m_onGraphicsData = onGraphicsData;
	m_offGraphicsData = offGraphicsData;
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
				//PERLog::Logger().Info("����ġ ��");

				// ������Ʈ�� ai ������Ʈ�� �ִ� Execute �Լ� ����
				dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Execute(*ativeObejct);
				object.GetGraphics().SetData(m_onGraphicsData);
			}
			else
			{
				//PERLog::Logger().Info("����ġ ����");
				// ������Ʈ�� ai ������Ʈ�� �ִ� Revoke �Լ� ����
				dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Revoke(*ativeObejct);
				object.GetGraphics().SetData(m_offGraphicsData);
			}
			m_isOn = (m_isOn + 1) % 2;
		}
		else
		{
			//PERLog::Logger().Info("��ư ����");
			// ������Ʈ�� ai ������Ʈ�� �ִ� Execute �Լ� ����
			dynamic_cast<ResponeseToSignalAiComponent*>(&ativeObejct->GetAi())->Execute(*ativeObejct);
			object.GetGraphics().SetData(m_onGraphicsData);
		}
	}
	m_isGetInput = false;
}