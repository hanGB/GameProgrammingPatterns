#pragma once
#include <map>
#include <string>
#include <unordered_set>

enum class EqSoundId {
	EQ_SOUND_BEEP,
	EQ_SOUND_CLICK,
	EQ_SOUND_CHECK_IN,
	EQ_SOUND_CHECK_OUT,
	EQ_SOUND_ID_NON
};

class EqSoundPlayer
{
public:
	static EqSoundPlayer& GetInstance();

	void Update(double elapsedTimeInSec);
	void PlaySound(EqSoundId id, double volume);
	void StopSound(EqSoundId id);
	void SetVolume(EqSoundId id, int volume);

private:
	EqSoundPlayer();
	~EqSoundPlayer();

	void ExecuteMessageInPendings();
	void RemoveOverSound();
	void RemovePlayingSound(int index);

	double MaxValueBetweenAandB(double a, double b);

	static const int c_MAX_PLAY_SOUND = 10;
	static const int c_PLAYING_TIME = 2;
	static const int c_MAX_PENDING = 16;

	int m_pendingHead = 0;
	int m_pendingTail = 0;
	struct PlayMessage {
		EqSoundId id;
		double volume;
	} m_pendings[c_MAX_PENDING];

	int m_numPlayingSound = 0;
	struct PlayingSound {
		EqSoundId id;
		std::string sound;
		double playingTime;
	} m_playingSounds[c_MAX_PLAY_SOUND];

	std::unordered_set<int> m_soundIdsInpeading;

	std::map<EqSoundId, std::string> m_soundInformationMap = {
		std::pair<EqSoundId, std::string>(EqSoundId::EQ_SOUND_BEEP, "sound beep"),
		std::pair<EqSoundId, std::string>(EqSoundId::EQ_SOUND_CLICK, "sound click"),
		std::pair<EqSoundId, std::string>(EqSoundId::EQ_SOUND_CHECK_IN, "sound check in"),
		std::pair<EqSoundId, std::string>(EqSoundId::EQ_SOUND_CHECK_OUT, "sound check out"),
	};
};