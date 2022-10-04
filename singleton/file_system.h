#pragma once
#include "stdafx.h"

class FileSystem {
public:
	static FileSystem& Instance() {
		static FileSystem* instance = new FileSystem();
		return *instance;
	}

private:
	FileSystem() {
		std::cout << "file_system is initalize.\n";
	}
};