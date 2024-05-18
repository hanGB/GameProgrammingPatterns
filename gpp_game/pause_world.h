#pragma once
#include "per_world.h"

class PauseWorld : public PERWorld {
public:
	PauseWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~PauseWorld();

private:
	virtual void InitWorldObject();
};