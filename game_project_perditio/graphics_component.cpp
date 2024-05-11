#include "stdafx.h"
#include "graphics_component.h"

GraphicsComponent::~GraphicsComponent()
{
	if (m_nextComponent) delete m_nextComponent;
}

void GraphicsComponent::Update(PERHud& hud, PERAudio& audio, double dTime)
{
	// 다음 컨포넌트 업데이트
	if ( m_nextComponent ) m_nextComponent->Update(hud, audio, dTime);
}

void GraphicsComponent::Render(PERRenderer& renderer, double frameGap)
{
	// 다음 컨포넌트 렌더링
	if ( m_nextComponent ) m_nextComponent->Render(renderer, frameGap);
}

void GraphicsComponent::SetData(PERComponent::GraphicsData data)
{
	// 다음 컨포넌트 데이터 설정
	if ( m_nextComponent ) m_nextComponent->SetData(data);
}

void GraphicsComponent::Initialize()
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->Initialize();
}

void GraphicsComponent::RemoveFloatingUi()
{
	// 다음 컨포넌트 플로팅 ui 삭제
	if ( m_nextComponent ) m_nextComponent->RemoveFloatingUi();
}

void GraphicsComponent::SetOwner(PERObject* object)
{
	m_owner = object;
	// 다음 컨포넌트의 소유자도 설정
	if ( m_nextComponent ) m_nextComponent->SetOwner(object);
}

void GraphicsComponent::SetNextComponent(GraphicsComponent* component)
{
	m_nextComponent = component;
}

void GraphicsComponent::SetPositionZValue(double z)
{
	m_positionZValue = z;
}

PERObject* GraphicsComponent::GetOwner()
{
	return m_owner;
}

GraphicsComponent* GraphicsComponent::GetNextComponent()
{
	return m_nextComponent;
}

double GraphicsComponent::GetPositionZValue() const
{
    return m_positionZValue;
}
