#pragma once
#include "stdafx.h"
#include "log.h"
#include "audio_player.h"

class GameObject {
protected:
	Log& GetLog() { return m_log; }

private:
	static Log& m_log;
};
Log& GameObject::m_log = *new Log();


class Character : GameObject {
public:
	void GiveSomeImformation() {
		GetLog().WriteLog("It's character.");
	}
};

class Game {
public:
	static Game& Instatnce() {
		static Game* m_instance = new Game();
		return *m_instance;
	}
	~Game() {
		delete m_log;
		delete m_audioPlayer;
	}

	Log& GetLog() { return *m_log; }
	AudioPlayer& GetAudioPlayer() { return *m_audioPlayer; }

	void SetLog(Log& log) {
		m_log = &log;
	}
	void SetAudioPlayer(AudioPlayer& audioPlayer) {
		m_audioPlayer = &audioPlayer;
	}


private:
	Log* m_log;
	AudioPlayer* m_audioPlayer;
};