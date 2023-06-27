#pragma once
#include "stdafx.h"
#include "per_event.h"

class EventReciver {
public:
	virtual void Recive(PEREvent event, PERVec3 data) = 0;
};