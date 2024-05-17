#pragma once
#include "per_world.h"

class PauseWorld : public PERWorld {
public:
	PauseWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~PauseWorld();

private:
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();
};