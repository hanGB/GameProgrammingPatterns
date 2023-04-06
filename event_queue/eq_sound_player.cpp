#include "stdafx.h"
#include "eq_sound_player.h"

EqSoundPlayer& EqSoundPlayer::GetInstance()
{
	static EqSoundPlayer instance;
	return instance;
}

void EqSoundPlayer::Update(double elapsedTimeInSec)
{
	ExecuteMessageInPendings();

	for (int i = 0; i < m_numPlayingSound; ++i) {
		m_playingSounds[i].playingTime -= elapsedTimeInSec;
	}

	int index = 0;
	while (true) {
		if (m_numPlayingSound == index) break;

		if (m_playingSounds[index].playingTime < 0.0) RemovePlayingSound(index);
		else index++;
	}
}

void EqSoundPlayer::PlaySound(EqSoundId id, double volume)
{
	assert(m_numPending < c_MAX_PENDING);

	m_pendings[m_numPending].id = id;
	m_pendings[m_numPending].volume = volume;
	m_numPending++;
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

void EqSoundPlayer::ExecuteMessageInPendings()
{
	for (int i = 0; i < m_numPending; ++i) {
		RemoveOverSound();
		std::string sound = m_soundInformationMap.find(m_pendings[i].id)->second;
		std::cout << "Play Sound: " << sound << " - volume " << m_pendings[i].volume << '\n';
		m_playingSounds[m_numPlayingSound].id = m_pendings[i].id;
		m_playingSounds[m_numPlayingSound].sound = sound;
		m_playingSounds[m_numPlayingSound].playingTime = (double)c_PLAYING_TIME;
		m_numPlayingSound++;
	}
	m_numPending = 0;
}

void EqSoundPlayer::RemoveOverSound()
{
	if (m_numPlayingSound < c_MAX_PLAY_SOUND) return;
	RemovePlayingSound(0);
}

void EqSoundPlayer::RemovePlayingSound(int index)
{
	std::string sound = m_playingSounds[index].sound;
	std::cout << "Sound: " << sound << " Removed\n";

	m_numPlayingSound--;
	if (index != (c_MAX_PLAY_SOUND - 1))
		memmove(&m_playingSounds[index], &m_playingSounds[index + 1], sizeof(PlayingSound) * (m_numPlayingSound - index));

	m_playingSounds[m_numPlayingSound].id = EqSoundId::EQ_SOUND_ID_NON;
}
