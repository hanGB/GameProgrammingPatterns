#pragma once

class PERController;
class PERRenderer;
class PERObject;
class ObjectPool;

class PERWorld {
public:
	PERWorld(PERObject* player, ObjectPool* objectPool);
	~PERWorld();

	void DoGarbegeCollection(double dTime);

	void InputUpdate(PERController& controller, double dTime);
	void AiUpdate(double dTime);
	void PhysicsUpdate(double dTime);
	void GraphicsUpdate(double dTime);
	void Render(PERRenderer& renderer);

	PERObject* AddAndGetObject(PERObjectType type);
	void AddObject(PERObject* object);
	void DeleteObject(PERObject* object);

private:
	void InitWorldObject(PERObject* player);
	void UpdateSortedObjects();

	ObjectPool* m_objectPool;

	std::vector<PERObject*> m_objects;
	int m_numObject = 0;

	std::vector<PERObject*> m_sortedObjects;
	bool m_isUpdateSortedObject = false;
};