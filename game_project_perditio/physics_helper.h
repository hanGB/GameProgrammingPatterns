#pragma once

class PERWorld;
class PERAudio;
class PERObject;
enum class PERObjectType;

enum class PERCollisionType {
	NONE,	
	FIXED,	
	MOVABLE,
	TRIGGER,
	NUM_COLLISION_TYPE
};

class PhysicsHelper {
public:
	// 월드 내 다른 오브젝트들과의 충돌 검사 
	static bool CheckCollisionBetweenOtherObjects(PERWorld& world, PERAudio& audio, PERObject& myObject, std::vector<PERObject*>& otherObjects, int numOtherObject,
		PERObject* exceptObject, double dTime);

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
	static bool IsCollisionIgnoreWithCollisionType(PERCollisionType myType, PERCollisionType otherType);
	static bool IsCollisionIgnoreWithObjectType(PERObjectType myObjectType, PERObjectType otherObjectType);
	static bool IsCollisionIgnoreWithPosition(PERVec3 myPos, PERVec3 otherPos);

	// AABB 충돌 관련
	static bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize,
		PERVec3 bPos, PERVec3 bSize);

	// AABB 충돌 처리
	static void ProcessCollision(PERWorld& world, PERAudio& audio, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, PERVec3 bPos, PERVec3 bSize, 
		std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime);
	
	// 위치 적용
	static void AdjustPositionBetweenObjects(PERWorld& world, PERAudio& audio, PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime);
	// Fixed와 Movable 사이 충돌 처리
	static void ProcessCollisionBetweenFixedAndMovable(
		PERWorld& world, PERAudio& audio, PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
		PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime);
	// Actor와 Movable 사이의 충돌 처리
	static void ProcessCollisionBetweenMovables(PERWorld& world, PERAudio& audio, PERObject& fastObject, PERVec3 fastPos, PERVec3 fastSize,
		PERObject& slowObject, PERVec3 slowPos, PERVec3 slowSize, std::vector<PERObject*>& otherObjects, int numOtherObject, double dTime);
	// 위치 이동 제외 충돌 처리
	static void ProcessCollisionWithoutMoving(PERWorld& world, PERAudio& audio, PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType,
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, double dTime);
};