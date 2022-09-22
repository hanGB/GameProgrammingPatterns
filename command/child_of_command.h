#pragma once
#include "stdafx.h"
#include "command.h"

class JumpCommand : public Command {
public:
	JumpCommand() {}
	virtual ~JumpCommand() {}

	virtual void Execute() {
		Jump();
	}

private:

	void Jump() {
		std::cout << "Jumping\n";
	};
};

class FireCommand : public Command {
public:
	FireCommand() {}
	virtual ~FireCommand() {}

	virtual void Execute() {
		Fire();
	}

private:

	void Fire() {
		std::cout << "Fireing\n";
	};
};

class ReloadCommand : public Command {
public:
	ReloadCommand() {}
	virtual ~ReloadCommand() {}

	virtual void Execute() {
		Reload();
	}

private:

	void Reload() {
		std::cout << "Reloading\n";
	};
};

class NullCommand : public Command {
public:
	NullCommand() {}
	virtual ~NullCommand() {}

	virtual void Execute() {}
};