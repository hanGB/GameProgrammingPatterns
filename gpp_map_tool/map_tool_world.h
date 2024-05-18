#pragma once
#include "per_world.h"

class ObjectSpawner;

class MapToolWorld : public PERWorld {
public:
	MapToolWorld(ObjectStorage* objectStorage, PERDatabase* database);
	~MapToolWorld();

	virtual void Enter(PERRenderer& renderer, PERAudio& audio);

private:
	virtual void InitWorldObject();

	void TestFixedObjects();
	void Test2FixedObjects();
};