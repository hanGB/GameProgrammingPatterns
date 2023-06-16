#pragma once

class PERController;
class PERRenderer;
class PERObject;
class ObjectFactory;

class PERWorld {
public:
	PERWorld(PERObject* player, ObjectFactory* objectFactory);
	~PERWorld();

	void Update(PERController& controller, double dTime);
	void Render(PERRenderer& renderer);

private:
	void InitWorldObject();

	PERObject* m_player;
	ObjectFactory* m_objectFactory;

	PERObject* m_wall1;
	PERObject* m_wall2;
};