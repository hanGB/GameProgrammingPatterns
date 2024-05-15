#include "stdafx.h"
#include "physics_component.h"

PhysicsComponent::~PhysicsComponent()
{
	if (m_nextComponent) delete m_nextComponent;
}

void PhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->Update(world, audio, dTime);
}

void PhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->SetData(data);
}

void PhysicsComponent::Initialize()
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->Initialize();
}

void PhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	// 다음 컨포넌트 초기화
	if ( m_nextComponent ) m_nextComponent->ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

void PhysicsComponent::SetOwner(PERObject* object)
{
	m_owner = object;
	// 다음 컨포넌트의 소유자도 설정
	if ( m_nextComponent ) m_nextComponent->SetOwner(object);
}

void PhysicsComponent::SetNextComponent(PhysicsComponent* component)
{
	m_nextComponent = component;
}

PERObject* PhysicsComponent::GetOwner()
{
	return m_owner;
}

PhysicsComponent* PhysicsComponent::GetNextComponent()
{
	return m_nextComponent;
}
