#pragma once

enum class PERSoundId {

};
enum class PERBGMId {

};

class PERAudio {
public:
	virtual ~PERAudio() {}

	virtual void PlaySound(PERSoundId id, double volume) = 0;
	virtual void StopSound(PERSoundId id) = 0;
	virtual void StopAllSounds() = 0;

	virtual void PlayBGM(PERBGMId id, double volume, bool isLoop) = 0;
	virtual void StopBGM(PERBGMId id) = 0;
	virtual void StopAllBGM() = 0;
};