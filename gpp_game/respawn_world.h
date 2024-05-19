#pragma once
#include "per_world.h"

class RespawnWorld : public PERWorld {
public:
	RespawnWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~RespawnWorld();

private:
	virtual void InitWorldObject();
};