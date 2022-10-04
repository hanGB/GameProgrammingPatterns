#include "stdafx.h"
#include "child_of_file_system.h"

int main()
{;
	char path[] = "path";
	char contents[] = "contents";
	FileSystem::Instance().ReadFile(path);
	FileSystem::Instance().WriteFile(path, contents);
}