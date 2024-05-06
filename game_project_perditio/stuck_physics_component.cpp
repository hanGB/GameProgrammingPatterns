#include "stdafx.h"
#include "stuck_physics_component.h"
#include "per_object.h"

void StuckPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// �θ� �پ� �ֵ��� ����
	PERObject* parent = GetOwner()->GetParent();

	// �θ� ���� ���
	if (!parent) GetOwner()->SetLifeTime(-1.0);
	// �θ� ���� ���
	else if (parent->GetLifeTime() <= 0.0) GetOwner()->SetLifeTime(-1.0);
	// �θ� �ְ� ����ִ� ���
	else {
		PERVec3 parentPos = parent->GetPosition();
		GetOwner()->SetPosition(parentPos + m_stuckPosition);
		world.CheckCollision(*GetOwner(), dTime);
	}
}

void StuckPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	m_stuckPosition = data.stuckPosition;
}

void StuckPhysicsComponent::Initialize(PERComponent::PhysicsData data)
{
	SetData(data);
}

void StuckPhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
}

void StuckPhysicsComponent::GiveForce(PERWorld& world, PERVec3 force, double dTime)
{
}

PERVec3 StuckPhysicsComponent::GetStuckPosition() const
{
	return m_stuckPosition;
}
