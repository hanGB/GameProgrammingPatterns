#pragma once

class PERController;
class PERRenderer;
class PERObject;
class ObjectPool;

class PERWorld {
public:
	PERWorld(PERObject* player, ObjectPool* objectPool);
	~PERWorld();

	void Update(PERController& controller, double dTime);
	void Render(PERRenderer& renderer);

	void AddObject(PERObject* object);
	void DeleteObject(int id);

private:
	void InitWorldObject();

	ObjectPool* m_objectPool;

	std::vector<PERObject*> m_objects;
	int m_numObject = 0;
};