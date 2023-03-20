#pragma once
#include "stdafx.h"

enum class SoundID {
	SOUND_SPROING,
	SOUND_SWOOP,
	SOUND_DIVE
};

class SoundPlayer {
public:
	SoundPlayer() { }
	~SoundPlayer() { }

	void PlaySound(SoundID id, double volume) {
		RemoveOverSound();
		std::string sound = m_soundInformationMap.find(id)->second;
		std::cout << "Play Sound: " << sound << " - volume " << volume << '\n';
		m_soundList.push_back(std::pair<SoundID, std::string>(id, sound));
	}
	void StopSound(SoundID id) {
		for (auto& it : m_soundList) {
			if (it.first == id) {
				std::string sound = it.second;
				std::cout << "Stop Sound: " << sound << '\n';
				m_soundList.remove(it);
				break;
			}
		}
	}
	void SetVolume(SoundID id, int volume) {
		for (auto& it : m_soundList) {
			if (it.first == id) {
				std::string sound = it.second;
				std::cout << "Sound: " << sound << " - volume " << volume << '\n';
			}
		}
	}

private:
	void RemoveOverSound() {
		if (m_soundList.size() < 10) return;

		auto it = m_soundList.begin();
		std::cout << it->second << ": removed\n";
		m_soundList.remove(*it);
	}

	// 최대 플레이 사운드 수
	const int c_MAX_PLAY_SOUND = 10;

	std::list<std::pair<SoundID, std::string>> m_soundList;
	std::map<SoundID, std::string> m_soundInformationMap = {
		std::pair<SoundID, std::string>(SoundID::SOUND_SPROING, "sound spring"),
		std::pair<SoundID, std::string>(SoundID::SOUND_SWOOP, "sound swoop"),
		std::pair<SoundID, std::string>(SoundID::SOUND_DIVE, "sound dive")
	};
};
