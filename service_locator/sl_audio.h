#pragma once

enum class SlSoundId {
	SOUND_BANG,
	SOUND_CLASH,
	END_OF_SOUND_ID
};

class SlAudio {
public:
	virtual ~SlAudio() {}
	virtual void PlaySound(SlSoundId id) = 0;
	virtual void StopSound(SlSoundId id) = 0;
	virtual void StopAllSounds() = 0;
};