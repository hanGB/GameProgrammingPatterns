#pragma once
#include "per_world.h"
#include "per_database.h"

class ObjectSpawner;

class TestWorld2 : public PERWorld {
public:
	TestWorld2(ObjectStorage* objectStorage, PERDatabase* database);
	~TestWorld2();

	// 해당 월드에 들어오고 나갈 때 처리
	virtual void Enter();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

private:
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();

	PERVec3 m_playerPosBeforePause;
};