#pragma once
#include "stdafx.h"

class AudioPlayer {
public:
	void Play(const char* name) {
		std::cout << "Start audio named " << name << std::endl;
	}
};