#pragma once

class PERController;
class PERRenderer;
class PERObject;
class ObjectPool;
class PERHud;

enum class PERWorldMessageId {
	WORLD_MESSAGE_ADD_OBJECT,
	WORLD_MESSAGE_DELETE_OBJECT,
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
	PERWorld(PERObject* player, ObjectPool* objectPool);
	~PERWorld();

	void Update(double dTime);

	void ObjectsInputUpdate(PERController& controller, double dTime);
	void ObjectsAiUpdate(double dTime);
	void ObjectsPhysicsUpdate(double dTime);
	void ObjectsGraphicsUpdate(double dTime);

	void UpdateSortedObjects();

	void Render(PERRenderer& renderer);

	// ��û�� �޴� �Լ�
	void RequestAddObject(
		PERObject* parent, PERObjectType type, 
		PERVec3 position, PERVec3 currentAccel, double lifeTime);
	void RequestDeleteObject(PERObject* object);
	
private:
	void InitWorldObject(PERObject* player);
	void DoGarbegeCollection(double dTime);
	void ProcessPendingMessage();

	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

	void ResizePedingArray();

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

	PERHud* m_hud;
};