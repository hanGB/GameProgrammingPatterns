#pragma once

class PERAudio;
class ObjectPool;
class PERController;
class PERRenderer;
class PERObject;
class GameMode;

enum class PERWorldMessageId {
	ADD_OBJECT,
	DELETE_OBJECT,
	NUM_WROLD_MESSAGE_ID
};

struct PERWorldMessage {
	PERWorldMessageId id;
	PERObject* object;
	PERObjectType type;
	PERVec3 position;
	PERVec3 currentAccel;
	PERStat stat;
	double lifeTime;
};

class PERWorld {
public:
	PERWorld(ObjectPool* objectPool, GameMode* mode);
	~PERWorld();

	void Update(PERAudio& audio, double dTime);
	void UIUpdate(PERAudio& audio, double dTime);

	void ObjectsInputUpdate(PERController& controller, PERAudio& audio, double dTime);
	void ObjectsAiUpdate(PERAudio& audio, double dTime);
	void ObjectsPhysicsUpdate(PERAudio& audio, double dTime);
	void ObjectsGraphicsUpdate(PERAudio& audio, double dTime);

	void UpdateSortedObjects();
	void UpdateCamera(PERRenderer& renderer, double frameGap);

	void Render(PERRenderer& renderer, double frameGap);
	void UIRender(PERRenderer& renderer);

	void Enter();
	void Exit();
	void Pause();
	void Resume();

	// 요청을 받는 함수
	void RequestAddObject(
		PERObject* parent, PERObjectType type, 
		PERVec3 position, PERVec3 currentAccel, PERStat stat, double lifeTime);
	void RequestDeleteObject(PERObject* object);

	// 충돌 확인
	bool CheckCollision(PERObject& object, double dTime);
	
private:
	void InitWorldObject();
	// 오브젝트 추가
	void AddFixedAndPhysicalObject();
	void AddOtherObject();

	void DoGarbegeCollection(double dTime);
	void ProcessPendingMessage();

	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	void ResizePedingArray();

	// AABB 충돌 관련
	bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize, 
		PERVec3 bPos, PERVec3 bSize);

	// AABB 충돌 처리
	// 위치 적용
	void AdjustPositionWithObjects(PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime);
	void ProcessCollisionBetweenFixedAndMovable(
		PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
		PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime);
	// 위치 이동 제외 충돌 처리
	void ProcessCollisionWithoutMoving(PERObject& aObject, PERObjectType aType, PERObject& bObject, PERObjectType bType);

	ObjectPool* m_objectPool;

	std::vector<PERObject*> m_objects;
	int m_numObject = 0;

	// 렌더링용 z좌표로 정렬된 오브젝트 벡터
	std::vector<PERObject*> m_sortedObjects;
	bool m_isUpdateSortedObject = false;

	// 메세지 처리 대기 배열(오브젝트 생성 삭제 관련)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;

	GameMode* m_gameMode;
};