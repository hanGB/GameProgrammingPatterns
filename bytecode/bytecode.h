#pragma once
#include "stdafx.h"

enum Instruction {
	INST_LITERAL		= 0x00,

	INST_SET_HEALTH		= 0x01,
	INST_SET_WISDOM		= 0x02,
	INST_SET_AGILITY	= 0x03,

	INST_GET_HEALTH		= 0x11,
	INST_GET_WISDOM		= 0x12,
	INST_GET_AGILITY	= 0x13,

	INST_PLAY_SOUND		= 0x21,
	INST_PLAY_PARTICLES = 0x22,

	INST_ADD			= 0x31,
	INST_SUB			= 0x32,
	INST_MULTI			= 0x33,
	INST_DIVI			= 0x34,
};

enum Sound {
	SOUND_BANG = 0x00
};

enum Particles {
	PARTICLE_FLAME = 0x00
};
