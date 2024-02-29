#pragma once

class PERAudio;
class ObjectPool;
class PERController;
class PERRenderer;
class PERObject;
class GameMode;
class PERDatabase;

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
	PERVec3 size;
	PERVec3 currentAccel;
	PERStat stat;
	double lifeTime;
};

class PERWorld {
public:
	PERWorld();
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

	virtual void Enter();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

	// ��û�� �޴� �Լ�
	void RequestAddObject(
		PERObject* parent, PERObjectType type, 
		PERVec3 position, PERVec3 size, PERVec3 currentAccel, PERStat stat, double lifeTime);
	void RequestDeleteObject(PERObject* object);

	// �浹 Ȯ��
	bool CheckCollision(PERObject& object, double dTime);
	
protected:
	// �ڽ��� �����ؼ� ����� �Լ�
	// ���� ����
	void InitSettingForWorld(ObjectPool* objectPool, PERDatabase* database, GameMode* mode);

	// ������Ʈ ���� ��� ����
	void SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	// ������Ʈ �߰� ���� ����
	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	ObjectPool* m_objectPool;
	PERDatabase* m_database;
	GameMode* m_gameMode;

private:
	void InitWorldObject();
	// ������Ʈ �߰�
	virtual void AddFixedAndPhysicalObjects() = 0;
	virtual void AddOtherObjects() = 0;

	// ������Ʈ ����
	virtual void DeleteWorldObjects() = 0;

	// ���� ������Ʈ
	virtual void WorldUpdate(PERAudio& audio, double dTime) = 0;

	void DoGarbegeCollection(double dTime);
	void ProcessPendingMessage();

	void ResizePedingArray();

	// AABB �浹 ����
	bool CheckAABBCollision(
		PERVec3 aPos, PERVec3 aSize, 
		PERVec3 bPos, PERVec3 bSize);

	// AABB �浹 ó��
	// ��ġ ����
	void AdjustPositionWithObjects(PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
		PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime);
	void ProcessCollisionBetweenFixedAndMovable(
		PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
		PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime);
	// ��ġ �̵� ���� �浹 ó��
	void ProcessCollisionWithoutMoving(PERObject& aObject, PERObjectType aType, PERObject& bObject, PERObjectType bType, double dTime);

	std::vector<PERObject*> m_objects;
	int m_numObject = 0;

	// �������� z��ǥ�� ���ĵ� ������Ʈ ����
	std::vector<PERObject*> m_sortedObjects;
	bool m_isUpdateSortedObject = false;

	// �޼��� ó�� ��� �迭(������Ʈ ���� ���� ����)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
};