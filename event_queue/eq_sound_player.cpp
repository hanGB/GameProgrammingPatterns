#include "stdafx.h"
#include "eq_sound_player.h"

EqSoundPlayer& EqSoundPlayer::GetInstance()
{
	static EqSoundPlayer instance;
	return instance;
}

void EqSoundPlayer::Update(double elapsedTimeInSec)
{
	for (int i = 0; i < m_playingSoundNum; ++i) {
		m_playingSounds[i].playingTime -= elapsedTimeInSec;
	}

	int index = 0;
	while (true) {
		if (m_playingSoundNum == index) break;

		if (m_playingSounds[index].playingTime < 0.0) RemovePlayingSound(index);
		else index++;
	}
}

void EqSoundPlayer::PlaySound(EqSoundId id, double volume)
{
	RemoveOverSound();
	std::string sound = m_soundInformationMap.find(id)->second;
	std::cout << "Play Sound: " << sound << " - volume " << volume << '\n';
	m_playingSounds[m_playingSoundNum].id = id;
	m_playingSounds[m_playingSoundNum].sound = sound;
	m_playingSounds[m_playingSoundNum].playingTime = c_PLAYING_TIME;
	m_playingSoundNum++;
}

void EqSoundPlayer::StopSound(EqSoundId id)
{
	for (int i = 0; i < c_MAX_PLAY_SOUND; ++i) {
		if (m_playingSounds[i].id == id) {
			std::string sound = m_playingSounds[i].sound;
			std::cout << "Stop Sound: " << sound << '\n';
			RemovePlayingSound(i);
		}
	}
}

void EqSoundPlayer::SetVolume(EqSoundId id, int volume)
{
	for (int i = 0; i < c_MAX_PLAY_SOUND; ++i) {
		if (m_playingSounds[i].id == id) {
			std::string sound = m_playingSounds[i].sound;
			std::cout << "Sound: " << sound << " - volume " << volume << '\n';
		}
	}

}

EqSoundPlayer::EqSoundPlayer()
{
}

EqSoundPlayer::~EqSoundPlayer()
{
}

void EqSoundPlayer::RemoveOverSound()
{
	if (m_playingSoundNum < c_MAX_PLAY_SOUND) return;
	RemovePlayingSound(0);
}

void EqSoundPlayer::RemovePlayingSound(int index)
{
	std::string sound = m_playingSounds[index].sound;
	std::cout << "Sound: " << sound << " Removed\n";

	m_playingSoundNum--;
	if (index != (c_MAX_PLAY_SOUND - 1))
		memmove(&m_playingSounds[index], &m_playingSounds[index + 1], sizeof(PlayingSound) * (m_playingSoundNum - index));

	m_playingSounds[m_playingSoundNum].id = EqSoundId::EQ_SOUND_ID_NON;
}
