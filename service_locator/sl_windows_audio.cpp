#include "stdafx.h"
#include "sl_windows_audio.h"

void SlWindowsAudio::PlaySound(SlSoundId id)
{
	RemoveOverSound();
	std::string sound = m_soundInformationMap.find(id)->second;
	std::cout << "Play Sound: " << sound << '\n';
	m_playingSounds[m_numPlayingSound].id = id;
	m_playingSounds[m_numPlayingSound].sound = sound;
	m_playingSounds[m_numPlayingSound].playingTime = (double)c_PLAYING_TIME;
	m_numPlayingSound++;
}

void SlWindowsAudio::StopSound(SlSoundId id)
{
	for (int i = 0; i < c_MAX_PLAY_SOUND; ++i) {
		if (m_playingSounds[i].id == id) {
			std::string sound = m_playingSounds[i].sound;
			std::cout << "Stop Sound: " << sound << '\n';
			RemovePlayingSound(i);
		}
	}
}

void SlWindowsAudio::StopAllSounds()
{
	for (int i = 0; i < m_numPlayingSound; ++i) {
		std::string sound = m_playingSounds[i].sound;
		std::cout << "Stop Sound: " << sound << '\n';
		m_playingSounds[i].id = SlSoundId::END_OF_SOUND_ID;
		m_playingSounds[i].sound.clear();
	}
	m_numPlayingSound = 0;
}

void SlWindowsAudio::RemoveOverSound()
{
	if (m_numPlayingSound < c_MAX_PLAY_SOUND) return;
	RemovePlayingSound(0);
}

void SlWindowsAudio::RemovePlayingSound(int index)
{
	std::string sound = m_playingSounds[index].sound;
	std::cout << "Sound: " << sound << " Removed\n";

	m_numPlayingSound--;
	if (index != (c_MAX_PLAY_SOUND - 1))
		memmove(&m_playingSounds[index], &m_playingSounds[index + 1], sizeof(PlayingSound) * (m_numPlayingSound - index));

	m_playingSounds[index].id = SlSoundId::END_OF_SOUND_ID;
	m_playingSounds[index].sound.clear();
}
