#include "parachute_falling_physics_component.h"
#include "physics_helper.h"
#include "per_object.h"

void ParachuteFallingPhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	// 낙하산으로 인해 등속으로 떨어짐 
	// y축을 수직으로 설정 
	// 필요 정보 얻기

	GetOwner()->SetVelocity(PERVec3(0.0, -m_fallingSpeed, 0.0));
	// 계산
	PhysicsHelper::CaculateMovementWithoutExternalForce(*GetOwner(), PERVec3(0.0, 0.0, 0.0), dTime);

	PhysicsComponent::Update(world, audio, dTime);
}

void ParachuteFallingPhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	PhysicsComponent::SetData(data);
}

void ParachuteFallingPhysicsComponent::Initialize()
{
	m_fallingSpeed = 5.0;
	PhysicsComponent::Initialize();
}

void ParachuteFallingPhysicsComponent::ProcessCollision(PERObject& collidedObject, 
	PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PhysicsComponent::ProcessCollision(collidedObject, collisionVelocity, changedVelocity, collisionTime);
}

void ParachuteFallingPhysicsComponent::SetFallingSpeed(double speed)
{
	m_fallingSpeed = speed;
}
