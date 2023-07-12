#pragma once
#include "stdafx.h"

enum class PEREvent {
	// game
	EVENT_RUN_DEFAULT_WORLD_AND_GAME_MODE,

	// ui
	EVENT_UPDATE_HP,
	EVENT_UPDATE_MP,

	NUM_EVENT
};
