#pragma once
#include "sl_audio.h"
#include "sl_null_audio.h"

class SlLocator{
public:
	static void Initialize();
	static SlAudio* GetAudio();
	static void ProvideAudio(SlAudio* audio);

private:
	static SlAudio* m_audio;
	static SlNullAudio m_nullAudio;
};
