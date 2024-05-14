#pragma once

class PERWorld;
class PERObject;
enum class PERObjectType;

enum class PERCollisionType {
	NONE,
	FIXED,
	MOVABLE,
	ACTOR,
	TRIGGER,
	BULLET,
	NUM_COLLISION_TYPE
};

class PhysicsHelper {
public:
	// 월드 내 다른 오브젝트들과의 충돌 검사 
	static bool CheckCollisionBetweenOtherObjects(PERWorld& world, PERObject& myObject, std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime);

	// 어떠한 외부 힘 없이 이동 계산
	static void CaculateMovementWithoutExternalForce(PERObject& object, PERVec3 currentAccel, double dTime);
	// 마찰력이 적용된 이동 계산
	static void CaculateMovementWithFriction(PERObject& object, PERVec3 currentAccel, double dTime);

	// 속도 계산
	static PERVec3 CaculateVelocityWithoutExternalForce(PERVec3 curentVelocity, PERVec3 accel, double dTime);
	// 위치 계산
	static PERVec3 CaculatePosition(PERVec3 currentPosition, PERVec3 velocity, PERVec3 accel, double dTime);

private:
	// 충돌 무시 검사
	static bool IsCollisionIgnoreWithId(PERObject& myObject, int myId, PERObject& otherObject, int otherId);
	static bool IsCollisionIgnoreWithCollisionType(PERObject& myObject, PERCollisionType myType, PERObject& otherObject, PERCollisionType otherType);
	static bool IsCollisionIgnoreWithPosition(PERObject& myObject, PERVec3 myPos, PERObject& otherObject, PERVec3 otherPos);

	// AABB 충돌 관련
	static bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize,
		PERVec3 bPos, PERVec3 bSize);
	static void ProcessCollisionBetweenFixedAndMovable(
		PERWorld& world, PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
		PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime);

	// AABB 충돌 처리
	static void ProcessCollision(PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, PERVec3 bPos, PERVec3 bSize, double dTime);
	// 위치 적용
	static void AdjustPositionWithObjects(PERWorld& world, PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime);
	// 위치 이동 제외 충돌 처리
	static void ProcessCollisionWithoutMoving(PERWorld& world, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType,
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, double dTime);
};