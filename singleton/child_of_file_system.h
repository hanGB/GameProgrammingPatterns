#pragma once
#include "file_system.h"

class PS4FileSystem : public FileSystem {
public:
	virtual char* ReadFile(char* path) override {
		std::cout << "Read file with sony file io api...\n";
		char contents[] = "contents";
		return contents;
	}

	virtual void WriteFile(char* path, char* contents) override {
		std::cout << "Write file with sony file io api...\n";
	}
};

class SwitchFileSystem : public FileSystem {
public:
	virtual char* ReadFile(char* path) override {
		std::cout << "Read file with nintendo file io api...\n";
		char contents[] = "contents";
		return contents;
	}

	virtual void WriteFile(char* path, char* contents) override {
		std::cout << "Write file with nintendo file io api...\n";
	}
};