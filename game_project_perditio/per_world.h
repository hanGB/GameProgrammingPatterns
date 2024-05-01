#pragma once

class PERAudio;
class ObjectStorage;
class PERController;
class PERRenderer;
class PERObject;
class GameMode;
class PERDatabase;
class PERHud;

enum class PERWorldMessageId {
	ADD_OBJECT,
	DELETE_OBJECT,
	NUM_WROLD_MESSAGE_ID
};

struct PERWorldMessage {
	PERWorldMessageId id;
	PERObject* object;
	PERObjectType type;
	std::string databaseId;
	bool isVisualId;
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
		PERObject* parent, PERObjectType type, const char* databaseId, bool isVisualId, PERStat stat,
		PERVec3 position, double lifeTime = PER_MAXIMUM_LIFE_TIME, PERVec3 currentAccel = PERVec3(0.0, 0.0, 0.0));
	void RequestDeleteObject(PERObject* object);

	// �浹 Ȯ��
	bool CheckCollision(PERObject& object, double dTime);

	PERHud& GetHud();
	PERDatabase& GetDatabase();
	
protected:
	// �ڽ��� �����ؼ� ����� �Լ�
	// ���� ����
	void InitSettingForWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode);

	// ������Ʈ ���־� ����
	void SetObjectVisual(PERObject* object, const char* visualId);

	// ������Ʈ ���� ��� ����
	void SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
		bool border = true, int borderWidth = 1, PERColor borderColor = PERColor(0, 0, 0));

	// ������Ʈ �߰� ���� ����
	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	ObjectStorage* m_objectStorage;
	PERDatabase* m_database;
	GameMode* m_gameMode;

private:
	void InitWorldObject();
	// ������Ʈ �߰�
	virtual void AddFixedAndPhysicalObjects() = 0;
	virtual void AddOtherObjects() = 0;

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
	int m_maxObject = PER_DEFAULT_MAX_OBJECTS;
	int m_numObject = 0;

	// �������� z��ǥ�� ���ĵ� ������Ʈ ����
	std::vector<PERObject*> m_sortedObjects;
	bool m_isUpdateSortedObject = false;

	// �޼��� ó�� ��� �迭(������Ʈ ���� ���� ����)
	int m_maxPending = PER_DEFAULT_MAX_EVENT_PENDING;
	PERWorldMessage* m_pending = new PERWorldMessage[PER_DEFAULT_MAX_EVENT_PENDING];
	int m_numPending = 0;
};