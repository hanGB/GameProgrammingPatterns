#include "stdafx.h"
#include "movable_physics_component.h"
#include "per_object.h"

void MovablePhysicsComponent::Update(PERObject& object, PERWorld& world, PERAudio& audio, double dTime)
{
	m_MoveFunc(*this, object, dTime);
	world.CheckCollision(object, dTime);
}

void MovablePhysicsComponent::SetData(PERComponent::PhysicsData data)
{
	if (data.friction) m_MoveFunc = &MovablePhysicsComponent::Move;
	else m_MoveFunc = &MovablePhysicsComponent::MoveWithoutFriction;
}

void MovablePhysicsComponent::ProcessCollision(PERObject& myObject, PERObject& otherObject, PERVec3 collisionVelocity, PERVec3 changedVelocity, double collisionTime)
{
	PERVec3 pos = myObject.GetPosition();
	PERVec3 vel = myObject.GetVelocity();

	// �浹�� �ð���ŭ �浹 �ӵ��� �ݴ�� �̵�
	pos.x = pos.x - collisionVelocity.x * collisionTime;
	pos.y = pos.y - collisionVelocity.y * collisionTime;

	myObject.SetPosition(pos);
	myObject.SetVelocity(changedVelocity);
}

void MovablePhysicsComponent::Move(PERObject& object, double dTime)
{
	// �ʿ� ���� ���
	PERVec3 pos = object.GetPosition();
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

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
	object.SetPosition(pos);
	object.SetVelocity(vel);

	// ���� ���ӵ� �ʱ�ȭ
	cAcc = PERVec3(0.0, 0.0, 0.0);
	object.SetCurrentAccel(cAcc);
}

void MovablePhysicsComponent::MoveWithoutFriction(PERObject& object, double dTime)
{
	// �ʿ� ���� ���
	PERVec3 pos = object.GetPosition();
	PERVec3 vel = object.GetVelocity();
	PERVec3 cAcc = object.GetCurrentAccel();
	double mass = object.GetMass();

	vel.x = vel.x + cAcc.x * dTime;
	vel.y = vel.y + cAcc.y * dTime;

	// ���� ��ġ ���
	pos.x = pos.x + vel.x * dTime + 0.5 * cAcc.x * dTime * dTime;
	pos.y = pos.y + vel.y * dTime + 0.5 * cAcc.y * dTime * dTime;

	// ��� ��� ����
	object.SetPosition(pos);
	object.SetVelocity(vel);

	// ���� ���ӵ� �ʱ�ȭ
	cAcc = PERVec3(0.0, 0.0, 0.0);
	object.SetCurrentAccel(cAcc);
}
