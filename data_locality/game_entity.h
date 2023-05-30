#pragma once
#include "stdafx.h"
#include "component.h"

class GameEntity {
public:
	GameEntity(AIComponent* ai, PhysicsComponent* physics, RenderComponent* render)
		: m_ai(ai), m_physics(physics), m_render(render)
	{

	}

	~GameEntity()
	{
		delete m_ai;
		delete m_physics;
		delete m_render;
	}

	AIComponent* GetAI() { return m_ai; }
	PhysicsComponent* GetPhysics() { return m_physics; }
	RenderComponent* GetRender() { return m_render; }

private:
	AIComponent* m_ai; 
	PhysicsComponent* m_physics; 
	RenderComponent* m_render;
};