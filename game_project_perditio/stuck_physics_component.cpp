#include "stdafx.h"
#include "stuck_physics_component.h"
#include "per_object.h"

void StuckPhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	// 부모에 붙어 있도록 설정
	PERObject* parent = object.GetParent();

	// 부모가 없는 경우
	if (!parent) object.SetLifeTime(-1.0);
	// 부모가 죽은 경우
	else if (parent->GetLifeTime() <= 0.0) object.SetLifeTime(-1.0);
	// 부모가 있고 살아있는 경우
	else {
		PERVec3 parentPos = parent->GetPosition();
		object.SetPosition(parentPos + m_stuckPosition);
		world.CheckCollision(object, dTime);
	}
}

void StuckPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	m_stuckPosition = data.stuckPosition;
}

void StuckPhysicsComponent::ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
}

void StuckPhysicsComponent::GiveForce(PERObject& object, PERWorld& world, PERVec3 force, double dTime)
{
}

PERVec3 StuckPhysicsComponent::GetStuckPosition() const
{
	return m_stuckPosition;
}
