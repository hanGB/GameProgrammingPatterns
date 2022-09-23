#pragma once
#include "stdafx.h"
#include "command.h"
#include "game_actor.h"

class JumpCommand : public Command {
public:
	JumpCommand() {}
	virtual ~JumpCommand() {}

	virtual void Execute(GameActor& actor) {
		actor.Jump();
	}

private:

};

class FireCommand : public Command {
public:
	FireCommand() {}
	virtual ~FireCommand() {}

	virtual void Execute(GameActor& actor) {
		actor.Fire();
	}

private:
};

class ReloadCommand : public Command {
public:
	ReloadCommand() {}
	virtual ~ReloadCommand() {}

	virtual void Execute(GameActor& actor) {
		actor.Reload();
	}

private:

};

class NullCommand : public Command {
public:
	NullCommand() {}
	virtual ~NullCommand() {}

	virtual void Execute(GameActor& actor) {}
};