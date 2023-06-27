#pragma once
#include "stdafx.h"

enum class PEREvent {
	EVENT_UPDATE_HP,
	EVENT_UPDATE_MP,
	NUM_EVENT
};

union PERUnionValue {
	PERVec3 vec;
	double d;
	int i;
};