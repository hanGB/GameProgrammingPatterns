#pragma once
#include "stdafx.h"

enum class PEREvent {
	// game
	EXECUTE_GAME,
	RUN_TEST_WORLD,
	RUN_TEST2_WORLD,
	CHANGE_WINDOW_SIZE,
	EXIT_GAME,

	// ui
	UPDATE_BD,
	UPDATE_MD,
	SET_SELECTED_MENU,

	// player_state
	ADD_BD_TO_PLAYER,

	// ̺Ʈ 
	NUM_EVENT
};
