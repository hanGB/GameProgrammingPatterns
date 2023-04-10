#pragma once
#include "sl_audio.h"

class SlLoggedAudio : public SlAudio {
public: 
	SlLoggedAudio(SlAudio& wrapped);

	virtual void PlaySound(SlSoundId id);
	virtual void StopSound(SlSoundId id);
	virtual void StopAllSounds();

private:
	void Log(const char* message);
	void LogWithSoundId(const char* message, SlSoundId id);

	SlAudio& m_wrapped;
};