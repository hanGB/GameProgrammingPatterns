#pragma once
#include "stdafx.h"
#include "gpp_world.h"

class GPPSpawner {
public:
	void SetGPPWorld(GPPWorld& world) {
		m_world = &world;
	}
	GPPWorld* GetGPPWorld() {
		return m_world;
	}

private:
	GPPWorld* m_world;
};