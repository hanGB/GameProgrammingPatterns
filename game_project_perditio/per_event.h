#pragma once
#include "stdafx.h"

enum class PEREvent {
	// game
	RUN_DEFAULT_WORLD,
	PUSH_CURRENT_WORLD_AND_RUN_TEST2_WORLD,
	CHANGE_WORLD,
	QUIT_WORLD,
	NOTING_TO_DO,

	// ui
	UPDATE_BD,
	UPDATE_MD,

	// player_state
	ADD_BD_TO_PLAYER,

	// ̺Ʈ 
	NUM_EVENT
};
