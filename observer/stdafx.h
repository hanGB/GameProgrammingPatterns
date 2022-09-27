#pragma once
#include <iostream>
#include <list>
#include <chrono>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

enum class Event {
	EVENT_ENTITY_FELL,
	EVENT_ENTITY_DEATH,
	SIZE
};

enum class Achievement {
	ACHIEVEMENT_FELL,
	ACHIEVEMENT_NO_DEATH,
	SIZE
};