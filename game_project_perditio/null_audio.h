#pragma once
#include "per_audio.h"

class NullAudio : public PERAudio {
public:
	virtual void LoadSound(PERSoundId id, const char* fileName) {}

	virtual void PlaySound(PERSoundId id, double volume) {}
	virtual void StopSound(PERSoundId id) {}
	virtual void StopAllSounds() {}

	virtual void SetBGM(PERSoundId id, double volume) {}
	virtual void AddBGM(PERSoundId id, double volume) {}
	virtual void PlayCurrentBGM() {}
	virtual void StopCurrentBGM() {}
	virtual void PauseCurrentBGM(bool isPause) {}
	virtual void StopAllBGMs() {}
};