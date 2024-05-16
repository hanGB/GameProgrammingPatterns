#pragma once
#include "per_audio.h"

class NullAudio : public PERAudio {
public:
	virtual void LoadSound(PERSoundId id, const char* fileName) {}

	// 일회성 소리(효과음)
	virtual void PlaySound(PERSoundId id, double volume) {}
	virtual void StopAllSounds() {}

	// 배경음악
	virtual void SetBGM(PERSoundId id, double volume) {}
	virtual void PlayBGM() {}
	virtual void StopBGM() {}
	virtual void PauseBGM(bool isPause) {}

	// 환경음
	virtual void SetAmbientSound(PERSoundId id, double volume, int slot) {}
	virtual void PlayAmbientSound(int slot) {}
	virtual void StopAmbientSound(int slot) {}
	virtual void PauseAmbientSound(bool isPause, int slot) {}
	virtual void PlayAllAmbientSounds() {}
	virtual void StopAllAmbientSounds() {}
	virtual void PauseAllAmbientSounds(bool isPause) {}
};