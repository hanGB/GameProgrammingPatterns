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
	PERObject* m_wall1;
	PERObject* m_wall2;
	ObjectFactory* m_objectFactory;

	/*std::vector<PERObject*>	m_objects;
	size_t m_numObject = 0;

	std::vector<PERAiComponent*>		m_aiComponents;
	std::vector<PERInputComponent*>		m_inputComponents;
	std::vector<PERPhysicsComponent*>	m_physicsComponents;
	std::vector<PERGraphicsComponent*>	m_graphicsComponents;
	size_t	m_numAiComponent = 0;
	size_t	m_numInputComponent = 0;
	size_t  m_numPhysicsComponent = 0;
	size_t  m_numGraphicsComponent = 0;*/
};