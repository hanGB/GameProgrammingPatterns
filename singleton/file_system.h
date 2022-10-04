#pragma once
#include "stdafx.h"

class FileSystem {
public:
	static FileSystem& Instance();

	virtual ~FileSystem() {}
	virtual char* ReadFile(char* path) = 0;
	virtual void WriteFile(char* path, char* contents) = 0;

protected:
	FileSystem() {}
};