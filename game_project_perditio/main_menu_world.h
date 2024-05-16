#pragma once
#include "per_world.h"

class MainMenuWorld : public PERWorld {
public:
	MainMenuWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~MainMenuWorld();

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