#include "file_system.h"
#include "child_of_file_system.h"

FileSystem& FileSystem::Instance()
{
#if PLATFORM == PLAYSTATION4
	static FileSystem* instance = new PS4FileSystem();
#elif PLATFORM == SWITCH
	static FileSystem* instance = new SwitchFileSystem();
#endif
	return *instance;
}