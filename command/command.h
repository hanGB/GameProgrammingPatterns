#pragma once
#include "stdafx.h"

class Command {
public:
	Command() {}
	virtual ~Command() {}

	virtual void Execute() = 0;
};
