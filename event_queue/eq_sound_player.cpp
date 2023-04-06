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
	if (m_soundIdsInpeading.find((int)id) != m_soundIdsInpeading.end()) {
		for (int i = m_pendingHead; i != m_pendingTail; i = (i + 1) % c_MAX_PENDING) {
			if (m_pendings[i].id == id) {
				m_pendings[i].volume = MaxValueBetweenAandB(m_pendings[i].volume, volume);
				return;
			}
		}
	}
	else {
		m_soundIdsInpeading.insert((int)id);
	}

	assert((m_pendingTail + 1) % c_MAX_PENDING  != m_pendingHead);

	m_pendings[m_pendingTail].id = id;
	m_pendings[m_pendingTail].volume = volume;
	m_pendingTail = (m_pendingTail + 1) % c_MAX_PENDING;
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
	if (m_pendingHead == m_pendingTail) return;

	RemoveOverSound();
	std::string sound = m_soundInformationMap.find(m_pendings[m_pendingHead].id)->second;
	std::cout << "Play Sound: " << sound << " - volume " << m_pendings[m_pendingHead].volume << '\n';
	m_playingSounds[m_numPlayingSound].id = m_pendings[m_pendingHead].id;
	m_playingSounds[m_numPlayingSound].sound = sound;
	m_playingSounds[m_numPlayingSound].playingTime = (double)c_PLAYING_TIME;
	m_soundIdsInpeading.erase((int)m_pendings[m_pendingHead].id);
	m_numPlayingSound++;
	m_pendingHead = (m_pendingHead + 1) % c_MAX_PENDING;
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

double EqSoundPlayer::MaxValueBetweenAandB(double a, double b)
{
	if (a > b) return a;
	else if (a < b) return b;
	return a;
}
