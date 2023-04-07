#pragma once
#include "sl_audio.h"

class SlLocator{
public:
	~SlLocator();

	static SlAudio* GetAudio();
	static void ProvideAudio(SlAudio* audio);

private:
	static SlAudio* m_audio;
};
