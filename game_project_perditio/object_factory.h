#pragma once
#include "per_object.h"

class ObjectFactory {
public:
	PERObject* CreatePlayer();
	PERObject* CreateWall();
	//PERObject* CreateFloor();

private:
	PERObject* CreateObject(PERObjectType inputType, PERObjectType aiType, PERObjectType physicsType, PERObjectType graphicsType);
};