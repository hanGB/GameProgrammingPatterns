#pragma once
#include "ai_component.h"


class MakingSignalAiComponent : public AiComponent {
public:
	virtual void Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::AiData data);
	virtual void Initialize(PERComponent::AiData data);

	void SetIsGetInput(bool input);
	void SetOnOffGraphicsData(PERComponent::GraphicsData onGraphicsData, PERComponent::GraphicsData offGraphicsData);

private:
	void MakeSignal(PERObject& object, PERAudio& audio, double dTime);

	// ��ư ����
	// �Է� ����
	bool m_isGetInput = false;
	// �Է� �� ��, ������ �ݺ��� 
	bool m_isSwitch = false;
	// �������� ����
	bool m_isOn = false;
	// ó�� �� �� �Է¸� ������
	bool m_isDisposable = false;

	// ����, ���� �׷��� ������
	PERComponent::GraphicsData m_onGraphicsData;
	PERComponent::GraphicsData m_offGraphicsData;
};