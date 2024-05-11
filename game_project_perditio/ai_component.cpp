#include "stdafx.h"
#include "ai_component.h"

AiComponent::~AiComponent()
{
	if (m_nextComponent) delete m_nextComponent;
}

void AiComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// 다음 컨포넌트 업데이트
	if ( m_nextComponent ) m_nextComponent->Update(world, audio, dTime);
}

void AiComponent::SetData(PERComponent::AiData data)
{
	// 다음 컨포넌트 데이터 설정
	if ( m_nextComponent ) m_nextComponent->SetData(data);
}

void AiComponent::Initialize()
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->Initialize();
}

void AiComponent::SetOwner(PERObject* object)
{
	m_owner = object;
	// 다음 컨포넌트의 소유자도 설정
	if ( m_nextComponent ) m_nextComponent->SetOwner(object);
}

void AiComponent::SetNextComponent(AiComponent* component)
{
	m_nextComponent = component;
}

PERObject* AiComponent::GetOwner()
{
	return m_owner;
}

AiComponent* AiComponent::GetNextComponent()
{
	return m_nextComponent;
}
