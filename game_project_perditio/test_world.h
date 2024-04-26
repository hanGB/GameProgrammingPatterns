#pragma once
#include "per_world.h"
#include "per_database.h"

class ObjectSpawner;

class TestWorld : public PERWorld {
public:
	TestWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode);
	~TestWorld();

	// �ش� ���忡 ������ ���� �� ó��
	virtual void Enter();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

private:
	// ������Ʈ �߰�
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();
};