#pragma once
#include "per_world.h"
#include "per_database.h"

class ObjectSpawner;

class TestWorld : public PERWorld {
public:
	TestWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~TestWorld();

	// 해당 월드에 들어오고 나갈 때 처리
	virtual void Enter(PERRenderer& renderer, PERAudio& audio);
	virtual void Exit(PERRenderer& renderer, PERAudio& audio);
	virtual void Pause(PERRenderer& renderer, PERAudio& audio);
	virtual void Resume(PERRenderer& renderer, PERAudio& audio);

private:
	virtual void InitWorldObject();
	void AddOtherObjects();

	PERVec3 m_playerPosBeforePause;
};