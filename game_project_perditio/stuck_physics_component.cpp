#include "stdafx.h"
#include "stuck_physics_component.h"
#include "per_object.h"

void StuckPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// 부모에 붙어 있도록 설정
	PERObject* parent = GetOwner()->GetParent();

	// 부모가 없는 경우
	if (!parent) GetOwner()->SetLifeTime(-1.0);
	// 부모가 죽은 경우
	else if (parent->GetLifeTime() <= 0.0) GetOwner()->SetLifeTime(-1.0);
	// 부모가 있고 살아있는 경우
	else {
		PERVec3 parentPos = parent->GetPosition();
		GetOwner()->SetPosition(parentPos + m_stuckPosition);
		world.CheckCollision(*GetOwner(), dTime);
	}

	PhysicsComponent::Update(world, audio, dTime);
}

void StuckPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	m_stuckPosition = data.stuckPosition;
	PhysicsComponent::SetData(data);
}

void StuckPhysicsComponent::Initialize()
{
	PhysicsComponent::Initialize();
}

void StuckPhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

void StuckPhysicsComponent::GiveForce(PERWorld& world, PERObject& opponentObject, PERVec3 force, double dTime)
{
	PhysicsComponent::GiveForce(world, opponentObject, force, dTime);
}

PERVec3 StuckPhysicsComponent::GetStuckPosition() const
{
	return m_stuckPosition;
}
