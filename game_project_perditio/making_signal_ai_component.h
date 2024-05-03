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

	// 버튼 정보
	// 입력 여부
	bool m_isGetInput = false;
	// 입력 시 온, 오프가 반복됨 
	bool m_isSwitch = false;
	// 켜졌는지 여부
	bool m_isOn = false;
	// 처음 한 번 입력만 반응함
	bool m_isDisposable = false;

	// 켜짐, 꺼짐 그래픽 데이터
	PERComponent::GraphicsData m_onGraphicsData;
	PERComponent::GraphicsData m_offGraphicsData;
};