#pragma once
#include "stdafx.h"
#include "entity.h"

class Observer {
public:
	virtual ~Observer() {}

	virtual void OnNotify(const Entity& entity, Event event) = 0;
};