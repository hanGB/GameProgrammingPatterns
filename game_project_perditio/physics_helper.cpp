#include "stdafx.h"
#include "physics_helper.h"
#include "stuck_physics_component.h"
#include "per_object.h"
#include "per_world.h"

bool PhysicsHelper::CheckCollisionBetweenOtherObjects(PERWorld& world, PERObject& myObject, std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime)
{
	// 충돌 처리 설정 -> type(충돌 검사를 요청한 오브젝트의 충돌 타입)은 fixed object, None이 될 수 없음
	// 
	// movable && movable == 완전 무시
	// trigger && movable == 완전 무시
	// trigger && bullet == 완전 무시
	// 
	// movable && fixed == fixed 오브젝트에 합쳐짐(변경 될 수 있음)
	// movable && Actor == 물리적 처리
	// Actor && fixed  == 물리적 처리
	// 
	// 나머지 관계 == 물리적 처리 없는 충돌 처리

	bool collided = false;
	bool isOnPlatform = false;

	// 본인 오브젝트 정보 얻기
	PERCollisionType type = myObject.GetCollisionType();
	PERVec3 position = myObject.GetBoundingBoxPosition(), size = myObject.GetBoundingBoxSize();
	PERBoundingType boundingtype = myObject.GetBoundingType();
	PERObjectType objectType = myObject.GetObjectType();

	int id = myObject.GetIDInWorld();
	for (int i = 0; i < numOtherObject; ++i) {
		PERObject& otherObject = *otherObjects[i];

		// 본인이 죽었으면 건너뜀
		if (myObject.GetLifeTime() < 0.0) return collided;
		// 아이디와 부모 등의 검사해서 건너뜀
		if (IsCollisionIgnoreWithId(myObject, id, otherObject, i)) continue;

		// 상대 콜리션 타입 정보 얻기
		PERCollisionType otherType = otherObjects[i]->GetCollisionType();
		// 충돌 타입을 검사해서 건너뜀
		if (IsCollisionIgnoreWithCollisionType(myObject, type, otherObject, otherType)) continue;

		// 상대 오브젝트 정보 얻기
		PERVec3 otherPos = otherObjects[i]->GetBoundingBoxPosition(), otherSize = otherObject.GetBoundingBoxSize();
		PERBoundingType otherBoundingType = otherObjects[i]->GetBoundingType();
		PERObjectType otherObjectType = otherObjects[i]->GetObjectType();

		// 몬스터끼리 충돌 무시
		if (objectType == PERObjectType::MONSTER && otherObjectType == PERObjectType::MONSTER) continue;
		// 위치를 검사해서 건너뜀
		if (IsCollisionIgnoreWithPosition(myObject, position, otherObject, otherPos)) continue;

		// 전에 충돌된 오브젝트가 같을 경우 충돌 오브젝트 제거
		if (myObject.GetCollidedObject() == &otherObject) myObject.SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
		if (&myObject == otherObject.GetCollidedObject()) otherObject.SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

		if (otherBoundingType == PERBoundingType::RECTANGLE && boundingtype == PERBoundingType::RECTANGLE)
		{
			if (CheckAABBCollision(position, size, otherPos, otherSize))
			{
				// 상대가 나보다 바로 아래 있고 고정 블럭일 경우 플랫폼 위에 있다고 설정 후 건너뜀(땅)
				if ((int)(floor(otherPos.z)) == (int)floor(position.z) - 1 && otherType == PERCollisionType::FIXED)
				{
					isOnPlatform = true;
					continue;
				}
				// 충돌로 설정
				collided = true;
				ProcessCollision(world, myObject, type, objectType, position, size, otherObject, otherType, otherObjectType, otherPos, otherSize, dTime);
			}
		}
	}

	// 플랫폼 위에 있지 않을 경우 떨어져 죽음
	if (!isOnPlatform) {
		myObject.GetObjectState().KillSelf(world);
	}

	return collided;
}

void PhysicsHelper::CaculateMovementWithoutExternalForce(PERObject& object, PERVec3 currentAccel, double dTime)
{
	// 필요 정보 얻기
	PERVec3 position = object.GetPosition();
	PERVec3 velocity = object.GetVelocity();
	double mass = object.GetMass();

	// 속도 계산
	velocity = CaculateVelocityWithoutExternalForce(velocity, currentAccel, dTime);
	// 위치 계산
	position = CaculatePosition(position, velocity, currentAccel, dTime);

	// 계산 결과 적용
	object.SetPosition(position);
	object.SetVelocity(velocity);
}

void PhysicsHelper::CaculateMovementWithFriction(PERObject& object, PERVec3 currentAccel, double dTime)
{
	// 필요 정보 얻기
	PERVec3 position = object.GetPosition();
	PERVec3 velocity = object.GetVelocity();
	double mass = object.GetMass();

	// 속도 계산
	if (std::abs(velocity.x) > 0.0) {
		currentAccel.x += velocity.x / std::abs(velocity.x) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelX = velocity.x + currentAccel.x * dTime;

		// 마찰력으로 인해 반대 방향으로 움직이지 않도록 확인
		if (tempVelX * velocity.x < 0.0) {
			velocity.x = 0.0;
		}
		else {
			velocity.x = tempVelX;
		}
	}
	else {
		velocity.x = velocity.x + currentAccel.x * dTime;
	}
	if (std::abs(velocity.y) > 0.0) {
		currentAccel.y += velocity.y / std::abs(velocity.y) * PER_FRICTION * (-PER_GRAVITY);

		double tempVelY = velocity.y + currentAccel.y * dTime;

		// 마찰력으로 인해 반대 방향으로 움직이지 않도록 확인
		if (tempVelY * velocity.y < 0.0) {
			velocity.y = 0.0;
		}
		else {
			velocity.y = tempVelY;
		}
	}
	else {
		velocity.y = velocity.y + currentAccel.y * dTime;
	}

	// 위치 계산
	position = CaculatePosition(position, velocity, currentAccel, dTime);

	// 계산 결과 적용
	object.SetPosition(position);
	object.SetVelocity(velocity);
}

PERVec3 PhysicsHelper::CaculateVelocityWithoutExternalForce(PERVec3 curentVelocity, PERVec3 accel, double dTime)
{
	PERVec3 caculatedVelocity;

	// z값은 그대로 유지
	caculatedVelocity.z = curentVelocity.z;

	caculatedVelocity.x = curentVelocity.x + accel.x * dTime;
	caculatedVelocity.y = curentVelocity.y + accel.y * dTime;

	return caculatedVelocity;
}

PERVec3 PhysicsHelper::CaculatePosition(PERVec3 currentPosition, PERVec3 velocity, PERVec3 accel, double dTime)
{
	PERVec3 caculatedPosition;
	
	// z값은 그대로 유지
	caculatedPosition.z = currentPosition.z;

	caculatedPosition.x = currentPosition.x + velocity.x * dTime + 0.5 * accel.x * dTime * dTime;
	caculatedPosition.y = currentPosition.y + velocity.y * dTime + 0.5 * accel.y * dTime * dTime;

	return caculatedPosition;
}


bool PhysicsHelper::IsCollisionIgnoreWithId(PERObject& myObject, int myId, PERObject& otherObject, int otherId)
{
	// 죽은 오브젝트 건너뜀
	if (otherObject.GetLifeTime() < 0.0) return true;

	// id가 같은 거(본인), 부모, 자식 건너뜀
	if (myId == otherId) return true;
	if (myObject.GetParent() == &otherObject) return true;
	if (&myObject == (otherObject.GetParent())) return true;

	return false;
}

bool PhysicsHelper::IsCollisionIgnoreWithCollisionType(PERObject& myObject, PERCollisionType myType, PERObject& otherObject, PERCollisionType otherType)
{
	if (otherType == PERCollisionType::NONE) return true;
	else if (otherType == PERCollisionType::TRIGGER) return true;
	else if (myType == PERCollisionType::MOVABLE && otherType == PERCollisionType::MOVABLE) return true;
	else if (myType == PERCollisionType::TRIGGER && (otherType == PERCollisionType::BULLET || otherType == PERCollisionType::FIXED)) return true;

	return false;
}

bool PhysicsHelper::IsCollisionIgnoreWithPosition(PERObject& myObject, PERVec3 myPos, PERObject& otherObject, PERVec3 otherPos)
{
	// 상대의 z값이 나보다 높으면 건너뜀
	if ((int)(floor(otherPos.z)) > (int)floor(myPos.z)) return true;
	// 상대의 z값 + 1이 나보다 작으면 건너뜀
	if ((int)(floor(otherPos.z)) + 1 < (int)floor(myPos.z)) return true;

	return false;
}

bool PhysicsHelper::CheckAABBCollision(PERVec3 aPos, PERVec3 aSize, PERVec3 bPos, PERVec3 bSize)
{
	PERVec3 aHalfSize = aSize * 0.5;
	PERVec3 bHalfSize = bSize * 0.5;

	if (aPos.x - aHalfSize.x > bPos.x + bHalfSize.x) return false;
	if (aPos.x + aHalfSize.x < bPos.x - bHalfSize.x) return false;
	if (aPos.y - aHalfSize.y > bPos.y + bHalfSize.y) return false;
	if (aPos.y + aHalfSize.y < bPos.y - bHalfSize.y) return false;

	return true;
}

void PhysicsHelper::ProcessCollision(PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, PERVec3 bPos, PERVec3 bSize, double dTime)
{
	// 물리적 처리 항목
	if (aType == PERCollisionType::MOVABLE && bType == PERCollisionType::FIXED)
		AdjustPositionWithObjects(world, aObject, aPos, aSize, bObject, bPos, bSize, dTime);
	else if (aType == PERCollisionType::MOVABLE && bType == PERCollisionType::ACTOR)
		AdjustPositionWithObjects(world, aObject, aPos, aSize, bObject, bPos, bSize, dTime);
	else if (aType == PERCollisionType::ACTOR && bType == PERCollisionType::MOVABLE)
		AdjustPositionWithObjects(world, aObject, aPos, aSize, bObject, bPos, bSize, dTime);
	else if (aType == PERCollisionType::ACTOR && bType == PERCollisionType::FIXED)
		AdjustPositionWithObjects(world, aObject, aPos, aSize, bObject, bPos, bSize, dTime);

	// 물리적 처리 없이 다른 상호 작용
	else ProcessCollisionWithoutMoving(world, aObject, aType, aObjectType, bObject, bType, bObjectType, dTime);
}

void PhysicsHelper::AdjustPositionWithObjects(PERWorld& world, PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime)
{
	// 이전 충돌로 인해 실제로 변경된 값 얻기
	PERVec3	aVel = aObject.GetCollidedVelocity(); double aMass = aObject.GetCollidedMass();
	PERVec3 bVel = bObject.GetCollidedVelocity(); double bMass = bObject.GetCollidedMass();

	// 충돌 처리(무거운 쪽을 고정된 걸로 생각)
	if (aMass > bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			world, aObject, aPos, aSize, aVel,
			bObject, bPos, bSize, bVel, dTime);
	}
	else if (aMass <= bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			world, bObject, bPos, bSize, bVel,
			aObject, aPos, aSize, aVel, dTime);
	}
}

void PhysicsHelper::ProcessCollisionBetweenFixedAndMovable(
	PERWorld& world, PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
	PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime)
{
	// 충돌 속도 계산
	PERVec3 collisionVelocity = movableVel - fixedVel;

	// 충돌 거리 계산
	PERVec3 fixedHalfSize = fixedSize * 0.5;
	PERVec3 movableHalfSize = movableSize * 0.5;

	PERVec3 collisionDistance = PERVec3(0.0, 0.0, 0.0);
	PERVec3 collisionTimeRate = PERVec3(0.0, 0.0, 0.0);

	if (collisionVelocity.x > 0.0) collisionDistance.x = (movablePos.x + movableHalfSize.x) - (fixedPos.x - fixedHalfSize.x);
	else if (collisionVelocity.x < 0.0) collisionDistance.x = (fixedPos.x + fixedHalfSize.x) - (movablePos.x - movableHalfSize.x);

	if (collisionVelocity.y > 0.0) collisionDistance.y = (movablePos.y + movableHalfSize.y) - (fixedPos.y - fixedHalfSize.y);
	else if (collisionVelocity.y < 0.0) collisionDistance.y = (fixedPos.y + fixedHalfSize.y) - (movablePos.y - movableHalfSize.y);

	if (collisionVelocity.x == 0.0) {
		if (collisionDistance.x == 0.0) collisionTimeRate.x = 0.0;
		else collisionTimeRate.x = 1.0;
	}
	else collisionTimeRate.x = std::abs(collisionDistance.x / collisionVelocity.x);

	if (collisionVelocity.y == 0.0) {
		if (collisionDistance.y == 0.0) collisionTimeRate.y = 0.0;
		else collisionTimeRate.y = 1.0;
	}
	else collisionTimeRate.y = std::abs(collisionDistance.y / collisionVelocity.y);

	double collisionTime = collisionTimeRate.x * dTime;
	if (collisionTimeRate.x < collisionTimeRate.y) collisionTime = collisionTimeRate.y * dTime;

	// 각 오브젝트에서 충돌 처리
	fixedObject.GetPhysics().ProcessCollision(movableObject, collisionVelocity, fixedVel, 0.0);
	movableObject.GetPhysics().ProcessCollision(fixedObject, collisionVelocity, PERVec3(0.0, 0.0, movableVel.z), dTime * 1.5);
}

void PhysicsHelper::ProcessCollisionWithoutMoving(PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType,
	PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, double dTime)
{
	// bullet && bullet == 물리적 처리만 무시, bullet 삭제
	// bullet && fixed == 물리적 처리만 무시, bullet 삭제
	// bullet && movable == 물리적 처리만 무시, bullet 삭제
	// bullet && actor == 물리적 처리만 무시, bullet 삭제, 데미지 처리
	// actor && actor == 물리적 처리만 무시, 데미지 처리		 
	// trigger && (actor || movable) == 물리적 처리만 무시
	// trigger && bullet == 완전 무시

	// 총알간 상쇄
	if (aType == PERCollisionType::BULLET && bType == PERCollisionType::BULLET) {
		aObject.SetLifeTime(-1.0);
		bObject.SetLifeTime(-1.0);
	}
	// 총알 데미지 처리
	else if (aType == PERCollisionType::BULLET) {
		bObject.GetObjectState().GiveDamage(aObject, world, aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);

		// 총알 속도 방향으로 약간 이동(넉백)
		if (aObjectType == PERObjectType::BULLET) {
			aObject.SetLifeTime(-1.0);
			bObject.GetPhysics().GiveForce(world, NormalizeVector(aObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// 칼날의 상대적 위치 방향으로 약간 이동(넉백)
		else if (aObjectType == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(aObject.GetPhysics());
			bObject.GetPhysics().GiveForce(world, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	else if (bType == PERCollisionType::BULLET) {
		aObject.GetObjectState().GiveDamage(bObject, world, bObject.GetObjectState().GetStat().physicalAttack, bObject.GetObjectState().GetStat().mindAttack);

		// 총알 속도 방향으로 약간 이동(넉백)
		if (bObjectType == PERObjectType::BULLET) {
			bObject.SetLifeTime(-1.0);
			aObject.GetPhysics().GiveForce(world, NormalizeVector(bObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// 칼날의 상대적 위치 방향으로 약간 이동(넉백)
		else if (bObjectType == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(bObject.GetPhysics());
			aObject.GetPhysics().GiveForce(world, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	// 나머지
	// 플레이어와 몬스터 간
	else if (aObjectType == PERObjectType::PLAYER && bObjectType == PERObjectType::MONSTER) {
		// 플레이어에게 대미지를 줌
		aObject.GetObjectState().GiveDamage(bObject, world, bObject.GetObjectState().GetCollisionDamage(), 0);
	}
	else if (bObjectType == PERObjectType::PLAYER && aObjectType == PERObjectType::MONSTER) {
		bObject.GetObjectState().GiveDamage(aObject, world, aObject.GetObjectState().GetCollisionDamage(), 0);
	}
	// 트리거와 플레이어 또는 몬스터 또는 움직이는 오브젝트간
	else if (aType == PERCollisionType::TRIGGER && (bType == PERCollisionType::ACTOR || bType == PERCollisionType::MOVABLE)) {
		// 충돌 처리(필요한 건 본인 오브젝트 뿐, 신호가 생긴 걸로 처리)
		aObject.GetPhysics().ProcessCollision(bObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	else if ((aType == PERCollisionType::ACTOR || aType == PERCollisionType::MOVABLE) && bType == PERCollisionType::TRIGGER) {
		bObject.GetPhysics().ProcessCollision(aObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}

}

