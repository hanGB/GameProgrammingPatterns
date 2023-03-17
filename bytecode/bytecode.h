#pragma once
#include "stdafx.h"

enum Instruction {
	INST_LITERAL		= 0x00,
	INST_SET_HEALTH		= 0x01,
	INST_SET_WISDOM		= 0x02,
	INST_SET_AGILITY	= 0x03,
	INST_PLAY_SOUND		= 0x04,
	INST_PLAY_PARTICLES = 0x05
};

enum Sound {
	SOUND_BANG = 0x00
};

enum Particles {
	PARTICLE_FLAME = 0x00
};
