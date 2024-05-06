#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"

void MovablePhysicsComponent::Update(PERWorld& world, PERAudio& audio, double dTime)
{
	m_MoveFunc(*this, dTime);
	if (!world.CheckCollision(*GetOwner(), dTime)) GetOwner()->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
	//else PERLog::Logger().Info("�浹��");
}

void MovablePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	if (data.friction) m_MoveFunc = &MovablePhysicsComponent::Move;
	else m_MoveFunc = &MovablePhysicsComponent::MoveWithoutFriction;
}

void MovablePhysicsComponent::Initialize(PERComponent::PhysicsData data)
{
	SetData(data);
}

void MovablePhysicsComponent::ProcessCollision(PERObject& collidedObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();

	// �浹�� �ð���ŭ �浹 �ӵ��� �ݴ�� �̵�
	pos.x = pos.x - collisionVelocity.x * collisionTime;
	pos.y = pos.y - collisionVelocity.y * collisionTime;

	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(changedVelocity);

	GetOwner()->SetCollidedObject(&collidedObject, collisionVelocity);
}

void MovablePhysicsComponent::GiveForce(PERWorld& world, PERVec3 force, double dTime)
{
	double mass = GetOwner()->GetMass();

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));
	GetOwner()->SetCurrentAccel(PERVec3(force.x / mass, force.y / mass, force.z / mass));

	// �̵� ��Ű�� �ٽ� ���
	m_MoveFunc(*this, dTime);
	if (!world.CheckCollision(*GetOwner(), dTime)) GetOwner()->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

	GetOwner()->SetVelocity(PERVec3(0.0, 0.0, 0.0));
}

void MovablePhysicsComponent::Move(double dTime)
{
	// �ʿ� ���� ���
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	double mass = GetOwner()->GetMass();

	// ������ ���
	if (std::abs(vel.x) > 0.0) {
		cAcc.x += vel.x / std::abs(vel.x) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelX = vel.x + cAcc.x * dTime;

		// ���������� ���� �ݴ� �������� �������� �ʵ��� Ȯ��
		if (tempVelX * vel.x < 0.0) {
			vel.x = 0.0;
		}
		else {
			vel.x = tempVelX;
		}
	}
	else {
		vel.x = vel.x + cAcc.x * dTime;
	}
	if (std::abs(vel.y) > 0.0) {
		cAcc.y += vel.y / std::abs(vel.y) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelY = vel.y + cAcc.y * dTime;

		// ���������� ���� �ݴ� �������� �������� �ʵ��� Ȯ��
		if (tempVelY * vel.y < 0.0) {
			vel.y = 0.0;
		}
		else {
			vel.y = tempVelY;
		}
	}
	else {
		vel.y = vel.y + cAcc.y * dTime;
	}

	// ���� ��ġ ���
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// ��� ��� ����
	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(vel);

	// ���� ���ӵ� �ʱ�ȭ
	cAcc = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(cAcc);
}

void MovablePhysicsComponent::MoveWithoutFriction(double dTime)
{
	// �ʿ� ���� ���
	PERVec3 pos = GetOwner()->GetPosition();
	PERVec3 vel = GetOwner()->GetVelocity();
	PERVec3 cAcc = GetOwner()->GetCurrentAccel();
	double mass = GetOwner()->GetMass();

	vel.x = vel.x + cAcc.x * dTime;
	vel.y = vel.y + cAcc.y * dTime;

	// ���� ��ġ ���
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// ��� ��� ����
	GetOwner()->SetPosition(pos);
	GetOwner()->SetVelocity(vel);

	// ���� ���ӵ� �ʱ�ȭ
	cAcc = PERVec3(0.0, 0.0, 0.0);
	GetOwner()->SetCurrentAccel(cAcc);
}
