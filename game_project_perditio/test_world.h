#pragma once
#include "per_world.h"

class ObjectSpawner;

class TestWorld : public PERWorld {
public:
	TestWorld(ObjectPool* objectPool, GameMode* mode);
	~TestWorld();

	// 해당 월드에 들어오고 나갈 때 처리
	virtual void Enter();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

private:
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();
	// 오브젝트 삭제
	virtual void DeleteWorldObjects();

	// 월드 업데이트
	virtual void WorldUpdate(PERAudio& audio, double dTime);

	// 오브젝트 스포너
	ObjectSpawner* m_monsterSpawners;
};