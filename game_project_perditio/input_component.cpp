#include "stdafx.h"
#include "input_component.h"

InputComponent::~InputComponent()
{
	if (m_nextComponent) delete m_nextComponent;
}

void InputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	// 다음 컨포넌트 업데이트
	if ( m_nextComponent ) m_nextComponent->Update(world, controller, audio, dTime);
}

void InputComponent::SetData(PERComponent::InputData data)
{
	// 다음 컨포넌트 데이터 설정
	if ( m_nextComponent ) m_nextComponent->SetData(data);
}

void InputComponent::Initialize()
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->Initialize();
}

void InputComponent::SetOwner(PERObject* object)
{
	m_owner = object;
	// 다음 컨포넌트의 소유자도 설정
	if ( m_nextComponent ) m_nextComponent->SetOwner(object);
}

void InputComponent::SetNextComponent(InputComponent* component)
{
	m_nextComponent = component;
}

PERObject* InputComponent::GetOwner()
{
	return m_owner;
}

InputComponent* InputComponent::GetNextComponent()
{
	return m_nextComponent;
}
