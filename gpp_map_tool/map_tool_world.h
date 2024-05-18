#pragma once
#include "per_world.h"

class ObjectSpawner;

class MapToolWorld : public PERWorld {
public:
	MapToolWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~MapToolWorld();

	virtual void Enter(PERRenderer& renderer, PERAudio& audio);

private:
	// 오브젝트 추가
	virtual void AddFixedAndPhysicalObjects();
	virtual void AddOtherObjects();

	void TestFixedObjects();
	void Test2FixedObjects();
	void TitleFixedObjects();
};