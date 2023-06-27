#pragma once
#include "stdafx.h"

enum class PEREvent {
	EVENT_SOMETHING,
	NUM_EVENT
};

union PERUnionValue {
	PERVec3 vec;
	double d;
	int i;
};