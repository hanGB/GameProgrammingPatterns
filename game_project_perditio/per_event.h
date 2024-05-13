#pragma once
#include "stdafx.h"

enum class PEREvent {
	// game
	EXECUTE_GAME,
	RUN_TEST_WORLD,
	RUN_TEST2_WORLD,

	// ui
	UPDATE_BD,
	UPDATE_MD,

	// player_state
	ADD_BD_TO_PLAYER,

	// ̺Ʈ 
	NUM_EVENT
};
