#include "stdafx.h"
#include "physics_helper.h"
#include "per_object.h"
#include "per_world.h"

bool PhysicsHelper::CheckCollisionBetweenOtherObjects(PERWorld& world, PERObject& myObject, std::vector<PERObject*>& otherObjects, int numOtherObject, PERObject* exceptObject, double dTime)
{
	// 충돌 처리 설정 -> 충돌 체크를 요청한 오브젝트(myObject)의 충돌 타입은 FIXED 나 NONE이 될 수 없음
	// ALL && NONE == 완전 무시
	// ALL && TRIGGER == 완전 무시(TRIGGER가 체크를 요청 했을 때만 처리)
	// movable && movable == 처리
	// movable && FIXED == 처리
	// TRIGGER && movable == 물리 무시
	// TRIGGER && FIXED  == 완전 무시

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
		// 제외 오브젝트인 경우 건너뜀
		if (&otherObject == exceptObject) continue;

		// 아이디와 부모 등의 검사해서 건너뜀
		if (IsCollisionIgnoreWithId(myObject, id, otherObject, i)) continue;

		// 상대 콜리션 타입 정보 얻기
		PERCollisionType otherType = otherObjects[i]->GetCollisionType();
		// 충돌 타입을 검사해서 건너뜀
		if (IsCollisionIgnoreWithCollisionType(type, otherType)) continue;

		// 상대 오브젝트 정보 얻기
		PERVec3 otherPos = otherObjects[i]->GetBoundingBoxPosition(), otherSize = otherObject.GetBoundingBoxSize();
		PERBoundingType otherBoundingType = otherObjects[i]->GetBoundingType();
		PERObjectType otherObjectType = otherObjects[i]->GetObjectType();

		// 오브젝트 타입을 검사해서 건너뜀
		if (IsCollisionIgnoreWithObjectType(objectType, otherObjectType)) continue;
		// 위치를 검사해서 건너뜀
		if (IsCollisionIgnoreWithPosition(position, otherPos)) continue;

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

				ProcessCollision(
					world, myObject, type, objectType, position, size, 
					otherObject, otherType, otherObjectType, otherPos, otherSize, otherObjects, 
					numOtherObject, dTime
				);
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

bool PhysicsHelper::IsCollisionIgnoreWithCollisionType(PERCollisionType myType, PERCollisionType otherType)
{
	if (otherType == PERCollisionType::NONE) return true;
	else if (otherType == PERCollisionType::TRIGGER) return true;
	else if (myType == PERCollisionType::TRIGGER && otherType == PERCollisionType::FIXED) return true;

	return false;
}

bool PhysicsHelper::IsCollisionIgnoreWithObjectType(PERObjectType myObjectType, PERObjectType otherObjectType)
{
	if (myObjectType == PERObjectType::MONSTER && otherObjectType == PERObjectType::MONSTER) return true;
	else if (myObjectType == PERObjectType::TRIGGER && otherObjectType == PERObjectType::BULLET) return true;
	else if (myObjectType == PERObjectType::TRIGGER && otherObjectType == PERObjectType::BLADE) return true;
	
	return false;
}

bool PhysicsHelper::IsCollisionIgnoreWithPosition(PERVec3 myPos, PERVec3 otherPos)
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

void PhysicsHelper::ProcessCollision(
	PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, PERVec3 bPos, PERVec3 bSize, 
	std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime
)
{
	// 물리적 처리 항목
	if (aType == PERCollisionType::MOVABLE && bType == PERCollisionType::FIXED)
		AdjustPositionBetweenObjects(world, aObject, aPos, aSize, bObject, bPos, bSize, dTime);
	else if (aType == PERCollisionType::MOVABLE && bType == PERCollisionType::MOVABLE)
	{
		PERVec3 aVel = aObject.GetVelocity();
		PERVec3 bvel = bObject.GetVelocity();

		if (aVel.GetSize() >= bvel.GetSize())
			ProcessCollisionBetweenMovables(world, aObject, aPos, aSize, bObject, bPos, bSize, otherObjects, numOtherObject, dTime);
		else
			ProcessCollisionBetweenMovables(world, bObject, bPos, bSize, aObject, aPos, aSize, otherObjects, numOtherObject, dTime);
	}

	// 물리적 처리 없이 다른 상호 작용
	ProcessCollisionWithoutMoving(world, aObject, aType, aObjectType, bObject, bType, bObjectType, dTime);
}

void PhysicsHelper::AdjustPositionBetweenObjects(PERWorld& world, PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime)
{
	// 이전 충돌로 인해 실제로 변경된 값 얻기
	PERVec3	aVel = aObject.GetVelocity(); double aMass = aObject.GetMass();
	PERVec3 bVel = bObject.GetVelocity(); double bMass = bObject.GetMass();

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

void PhysicsHelper::ProcessCollisionBetweenMovables(PERWorld& world, PERObject& fastObject, PERVec3 fastPos, PERVec3 fastSize,
	PERObject& slowObject, PERVec3 slowPos, PERVec3 slowSize, std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime)
{
	PERVec3 fastVel = fastObject.GetVelocity();
	PERVec3 slowVel = slowObject.GetVelocity();

	// 일단 빠른 오브젝트를 고정으로 느린 오브젝트를 이동시킴
	ProcessCollisionBetweenFixedAndMovable(world, fastObject, fastPos, fastSize, fastVel, slowObject, slowPos, slowSize, slowVel, dTime);
	// 느린 오브젝트의 속도를 빠른 오브젝트의 속도로 변경
	slowObject.SetVelocity(fastVel);
	// 느린 오브젝트를 다시 충돌 처리(현재 충돌된 오브젝트(빠른 오브젝트) 제외하고 나머지 오브젝트들)
	bool IsSlowObjectCollidedOtherObjects = CheckCollisionBetweenOtherObjects(world, slowObject, otherObjects, numOtherObject, &fastObject, dTime);

	// 느린 오브젝트 속도를 기존값으로 되돌림(왠만해서는 기존 값이 PERVec3(0.0, 0.0, 0.0)일 가능성이 높음)
	slowObject.SetVelocity(slowVel);
	

	// 느린 오브젝트 충돌 처리 이후 빠른 오브젝트와 충돌될 경우 느린 오브젝트를 고정으로 빠른 오브젝트를 이동시킴
	if (IsSlowObjectCollidedOtherObjects) 
	{
		// 충돌이 있었을 경우  느린 오브젝트의 위치가 변경되었을 가능성 존재
		slowPos = slowObject.GetPosition();

		if (CheckAABBCollision(fastPos, fastSize, slowPos, slowSize)) 
		{
			ProcessCollisionBetweenFixedAndMovable(world, slowObject, slowPos, slowSize, slowVel, fastObject, fastPos, fastSize, fastVel, dTime);
		}
	}	
}

void PhysicsHelper::ProcessCollisionWithoutMoving(PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType,
	PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, double dTime)
{
	// 총알간 상쇄
	if ((aObjectType == PERObjectType::BULLET || aObjectType == PERObjectType::BLADE) 
		&& (bObjectType == PERObjectType::BULLET || bObjectType == PERObjectType::BLADE)) {
		aObject.SetLifeTime(-1.0);
		bObject.SetLifeTime(-1.0);
	}
	// 총알, 칼날 데미지 처리
	else if (aObjectType == PERObjectType::BULLET || aObjectType == PERObjectType::BLADE) {
		bObject.GetObjectState().GiveDamage(aObject, world, aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);
		// 총알 삭제
		if (aObjectType == PERObjectType::BULLET) {
			aObject.SetLifeTime(-1.0);
		}
	}
	else if (bObjectType == PERObjectType::BULLET || bObjectType == PERObjectType::BLADE) {
		aObject.GetObjectState().GiveDamage(bObject, world, bObject.GetObjectState().GetStat().physicalAttack, bObject.GetObjectState().GetStat().mindAttack);	
		// 총알 삭제
		if (bObjectType == PERObjectType::BULLET) {
			bObject.SetLifeTime(-1.0);
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
	// 트리거와 움직이는 오브젝트간
	else if (aType == PERCollisionType::TRIGGER && bType == PERCollisionType::MOVABLE) {
		// 충돌 처리(필요한 건 본인 오브젝트 뿐, 신호가 생긴 걸로 처리)
		aObject.GetPhysics().ProcessCollision(bObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	// a 오브젝트가 충돌 체크을 요청한 오브젝트이기 때문에 사실상 없는 경우
	else if (aType == PERCollisionType::MOVABLE && bType == PERCollisionType::TRIGGER) {
		bObject.GetPhysics().ProcessCollision(aObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}

}

