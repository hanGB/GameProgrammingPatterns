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

	void Update(PERAudio& audio, double dTime);
	void UIUpdate(PERAudio& audio, double dTime);

	void ObjectsInputUpdate(PERController& controller, PERAudio& audio, double dTime);
	void ObjectsAiUpdate(PERAudio& audio, double dTime);
	void ObjectsPhysicsUpdate(PERAudio& audio, double dTime);
	void ObjectsGraphicsUpdate(PERAudio& audio, double dTime);

	void UpdateSortedObjects();

	void Render(PERRenderer& renderer, double frameGap);
	void UIRender(PERRenderer& renderer);

	void Enter();
	void Exit();
	void Pause();
	void Resume();

	// ��û�� �޴� �Լ�
	void RequestAddObject(
		PERObject* parent, PERObjectType type, 
		PERVec3 position, PERVec3 currentAccel, double lifeTime);
	void RequestDeleteObject(PERObject* object);

	// �浹 Ȯ��
	bool CheckCollision(PERObject& object, PERVec3 position, PERVec3 size, PERVec3 velocity, PERBoundingType type);
	
private:
	void InitWorldObject();
	void DoGarbegeCollection(double dTime);
	void ProcessPendingMessage();

	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	void ResizePedingArray();

	bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize, PERVec3 aVel, 
		PERVec3 bPos, PERVec3 bSize, PERVec3 bVel);

	ObjectPool* m_objectPool;

	std::vector<PERObject*> m_objects;
	int m_numObject = 0;

	// �������� z��ǥ�� ���ĵ� ������Ʈ ����
	std::vector<PERObject*> m_sortedObjects;
	bool m_isUpdateSortedObject = false;

	// �޼��� ó�� ��� �迭(������Ʈ ���� ���� ����)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;

	GameMode* m_gameMode;
};