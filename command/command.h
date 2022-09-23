#pragma once
#include "stdafx.h"
#include "game_actor.h"

class Command {
public:
	virtual ~Command() {}
	virtual void Execute() = 0;
	virtual void Undo() = 0;
};
