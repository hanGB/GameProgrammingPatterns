#pragma once
#include "per_world.h"

class TitleWorld : public PERWorld {
public:
	TitleWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~TitleWorld();

	// 해당 월드에 들어오고 나갈 때 처리
	virtual void Enter(PERRenderer& renderer, PERAudio& audio);
	virtual void Exit(PERRenderer& renderer, PERAudio& audio);
	virtual void Pause(PERRenderer& renderer, PERAudio& audio);
	virtual void Resume(PERRenderer& renderer, PERAudio& audio);

private:
	virtual void InitWorldObject();
};