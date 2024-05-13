#pragma once

class PERAudio;
class ObjectStorage;
class PERController;
class PERRenderer;
class PERObject;
class GameMode;
class PERDatabase;
class PERHud;
class PERParticlePool;
class InputComponent;
class AiComponent;
class PhysicsComponent;
class GraphicsComponent;
enum class PERObjectType;
enum class PERCollisionType;

enum class PERWorldMessageId {
	ADD_OBJECT,
	DELETE_OBJECT,
	SLEEP_OBJECT,
	WAKE_UP_OBJECT,
	NUM_WROLD_MESSAGE_ID
};

enum class PERWorldType {
	MAIN_MENU,
	TEST_WORLD,
	TEST_WORLD2,
	NUM_WORLD_TYPE
};

struct PERWorldMessage {
	PERWorldMessageId id;
	PERObject* object;
	PERObjectType type;
	std::string databaseId;
	PERDatabaseType databaseType;
	PERVec3 position;
	PERVec3 currentAccel;
	PERStat stat;
	double lifeTime;
};

class PERWorld {
public:
	PERWorld();
	~PERWorld();

	void Update(PERAudio& audio, double dTime);
	void UIUpdate(PERController& controller, PERAudio& audio, double dTime);

	void InputUpdate(PERController& controller, PERAudio& audio, double dTime);
	void AiUpdate(PERAudio& audio, double dTime);
	void PhysicsUpdate(PERAudio& audio, double dTime);
	void GraphicsUpdate(PERAudio& audio, double dTime);

	void UpdateSortedGraphicsComponents();
	void UpdateCamera(PERRenderer& renderer, double frameGap);

	void Render(PERRenderer& renderer, double frameGap);
	void UIRender(PERRenderer& renderer);

	virtual void Enter();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

	// 요청을 받는 함수
	void RequestAddObject(
		PERObject* parent, PERObjectType type, const char* databaseId, PERDatabaseType databaseType, PERStat stat,
		PERVec3 position, double lifeTime = PER_MAXIMUM_LIFE_TIME, PERVec3 currentAccel = PERVec3(0.0, 0.0, 0.0));
	void RequestDeleteObject(PERObject* object);

	// 충돌 확인
	bool CheckCollision(PERObject& object, double dTime);

	PERHud& GetHud();
	PERDatabase& GetDatabase();
	PERParticlePool& GetParticlePool();
	GameMode& GetGameMode();
	
protected:
	// 자식이 접근해서 사용할 함수
	// 월드 설정
	void InitSettingForWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode);

	// 오브젝트 비주얼 설정
	void SetObjectVisual(PERObject* object, const char* visualId);

	// 오브젝트 색상 모양 설정
	void SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	// 오브젝트 추가 삭제 관련
	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	ObjectStorage* m_objectStorage;
	PERDatabase* m_database;
	GameMode* m_gameMode;

private:
	void InitWorldObject();
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects() = 0;
	virtual void AddOtherObjects() = 0;

	void DoGarbegeCollection(double dTime);
	void InformObjectDeadToSpawner(PERObject* object);
	void SleepAndWakeupObjects();
	void ProcessPendingMessage();

	void RequestSimpleDoObject(PERObject* object, const PERWorldMessageId& message);

	// Add Message  처리
	void ProcessAddMessage(PERWorldMessage& message);
	void SetBaseOfAddMessage(PERWorldMessage& message, PERObject* newObject, VisualData* vData);
	void SetForAddParticleEffecterMessage(PERWorldMessage& message, PERObject* newObject, EffectData* eData);
	void SetForAddBladeMessage(PERWorldMessage& message, PERObject* newObject);
	void SetForAddByObjectMessage(PERWorldMessage& message, PERObject* newObject);

	// 오브젝트 잠들게 함
	void SleepObject(PERObject* object);
	// 오브젝트 깨움
	void WakeUpObject(PERObject* object);

	void ResizePedingArray();

	// 충돌 무시 검사
	bool IsCollisionIgnoreWithId(PERObject& myObject, int myId, PERObject& otherObject, int otherId) const;
	bool IsCollisionIgnoreWithCollisionType(PERObject& myObject, PERCollisionType myType, PERObject& otherObject, PERCollisionType otherType) const;
	bool IsCollisionIgnoreWithPosition(PERObject& myObject, PERVec3 myPos, PERObject& otherObject, PERVec3 otherPos) const;

	// AABB 충돌 관련
	bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize, 
		PERVec3 bPos, PERVec3 bSize);
	void ProcessCollisionBetweenFixedAndMovable(
		PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
		PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime);
	
	// AABB 충돌 처리
	void ProcessCollision(PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, PERVec3 bPos, PERVec3 bSize, double dTime);
	// 위치 적용
	void AdjustPositionWithObjects(PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime);
	// 위치 이동 제외 충돌 처리
	void ProcessCollisionWithoutMoving(PERObject& aObject, PERCollisionType aType, PERObjectType aObjectType, 
		PERObject& bObject, PERCollisionType bType, PERObjectType bObjectType, double dTime);

	// 반환
	void ReturnObejctToStorage();
	void ClearObejctVector();

	std::vector<PERObject*> m_objects;
	int m_maxObject = PER_DEFAULT_MAX_OBJECTS;
	int m_numObject = 0;
	// 컨포넌트
	std::vector<InputComponent*> m_inputComponents;
	std::vector<AiComponent*> m_aiComponents;
	std::vector<PhysicsComponent*> m_physicsComponents;
	std::vector<GraphicsComponent*> m_graphicsComponents;

	// 렌더링용 z좌표로 정렬된 그래픽스 컨포넌트
	std::vector<GraphicsComponent*> m_sortedGraphicsComponents;
	bool m_isUpdateSortedGraphicsComponent = false;

	// 슬립 오브젝트 보관
	std::vector<PERObject*> m_sleepObjects;
	int m_maxSleepObject = PER_DEFAULT_MAX_OBJECTS;
	int m_numSleepObject = 0;

	// 이펙트용 파티클 풀
	PERParticlePool* m_particlePool;

	// 메세지 처리 대기 배열(오브젝트 생성 삭제 관련)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
};