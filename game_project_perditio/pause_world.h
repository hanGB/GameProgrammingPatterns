#pragma once
#include "per_world.h"

class PauseWorld : public PERWorld {
public:
	PauseWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~PauseWorld();

	// 해당 월드에 들어오고 나갈 때 처리
	virtual void Enter(PERAudio& audio);
	virtual void Exit(PERAudio& audio);
	virtual void Pause(PERAudio& audio);
	virtual void Resume(PERAudio& audio);

private:
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();
};