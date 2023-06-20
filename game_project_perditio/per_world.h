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

private:
	void InitWorldObject();

	PERObject* m_player;
	PERObject* m_wall1;
	PERObject* m_wall2;
	ObjectPool* m_objectPool;

};