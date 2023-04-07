#pragma once
#include "sl_audio.h"
#include <map>
#include <string>

class SlWindowsAudio : public SlAudio {
public:
	virtual void PlaySound(SlSoundId id);
	virtual void StopSound(SlSoundId id);
	virtual void StopAllSounds();

private:
	void RemoveOverSound();
	void RemovePlayingSound(int index);


	static const int c_MAX_PLAY_SOUND = 10;
	static const int c_PLAYING_TIME = 2;

	struct PlayingSound {
		SlSoundId id;
		std::string sound;
		double playingTime;
	} m_playingSounds[c_MAX_PLAY_SOUND];
	int m_numPlayingSound = 0;


	std::map<SlSoundId, std::string> m_soundInformationMap = {
		std::pair<SlSoundId, std::string>(SlSoundId::SOUND_BANG, "sound bang"),
		std::pair<SlSoundId, std::string>(SlSoundId::SOUND_CLASH, "sound clash"),
	};
};