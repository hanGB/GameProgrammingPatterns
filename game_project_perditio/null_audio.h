#pragma once
#include "per_audio.h"

class NullAudio : public PERAudio {
public:
	virtual void Update() {}

	virtual void PlaySound(PERSoundId id, double volume) {}
	virtual void StopSound(PERSoundId id) {}
	virtual void StopAllSounds() {}

	virtual void PlayBGM(PERBGMId id, double volume, bool isLoop) {}
	virtual void StopBGM(PERBGMId id) {}
	virtual void StopAllBGM() {}
};