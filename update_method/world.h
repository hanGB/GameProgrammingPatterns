#pragma once
#include "entity.h"
#include "renderer.h"

class World {
public:
	World() : m_numEntities(0), m_tempNumEntities(0), m_renderer(new Renderer()) { Init(); }
	~World() { DeleteAllEntities(); delete m_renderer; }

	void GameLoop();
	void AddEntityAfterOneLoopEnd(Entity* entity);

private:
	void Init();
	void UpdateEntitiesList();
	void DeleteAllEntities();
	void RenderWall();

	Entity* m_entities[MAX_ENTITIES] = { nullptr, };
	Entity* m_tempEntities[MAX_ENTITIES];
	int		m_numEntities;
	int		m_tempNumEntities;

	Renderer* m_renderer;
};