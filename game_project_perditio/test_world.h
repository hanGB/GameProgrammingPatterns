#pragma once
#include "per_world.h"

class ObjectSpawner;

class TestWorld : public PERWorld {
public:
	TestWorld(ObjectPool* objectPool, GameMode* mode);
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
	// ������Ʈ ����
	virtual void DeleteWorldObjects();

	// ���� ������Ʈ
	virtual void WorldUpdate(PERAudio& audio, double dTime);

	// ������Ʈ ������
	ObjectSpawner* m_monsterSpawners;
};