#pragma once
#include "stdafx.h"
#include "game_actor.h"

class Command {
public:
	virtual ~Command() {}
	virtual void Execute(GameActor& actor) = 0;
};
