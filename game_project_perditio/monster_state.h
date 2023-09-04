#pragma once

#include "object_state.h"

class MonsterState : public ObjectState {
	virtual bool GiveDamage(PERObject& object, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);
};