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
	double lifeTime;
};

class PERWorld {
public:
	PERWorld(ObjectPool* objectPool, GameMode* mode);
	~PERWorld();

	void Update(PERAudio* audio, double dTime);
	void UIUpdate(PERAudio* audio, double dTime);

	void ObjectsInputUpdate(PERController& controller, PERAudio* audio, double dTime);
	void ObjectsAiUpdate(PERAudio* audio, double dTime);
	void ObjectsPhysicsUpdate(PERAudio* audio, double dTime);
	void ObjectsGraphicsUpdate(PERAudio* audio, double dTime);

	void UpdateSortedObjects();

	void Render(PERRenderer& renderer, double frameGap);
	void UIRender(PERRenderer& renderer);

	void Enter();
	void Exit();
	void Pause();
	void Resume();

	// 요청을 받는 함수
	void RequestAddObject(
		PERObject* parent, PERObjectType type, 
		PERVec3 position, PERVec3 currentAccel, double lifeTime);
	void RequestDeleteObject(PERObject* object);
	
private:
	void InitWorldObject();
	void DoGarbegeCollection(double dTime);
	void ProcessPendingMessage();

	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	void ResizePedingArray();

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