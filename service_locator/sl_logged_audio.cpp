#include "stdafx.h"
#include "sl_logged_audio.h"

SlLoggedAudio::SlLoggedAudio(SlAudio& wrapped)
	: m_wrapped(wrapped)
{

}

void SlLoggedAudio::PlaySound(SlSoundId id)
{
	LogWithSoundId("Play sound", id);
	m_wrapped.PlaySound(id);
}

void SlLoggedAudio::StopSound(SlSoundId id)
{
	LogWithSoundId("Stop sound", id);
	m_wrapped.StopSound(id);
}

void SlLoggedAudio::StopAllSounds()
{
	Log("Stop all sounds");
	m_wrapped.StopAllSounds();
}

void SlLoggedAudio::Log(const char* message)
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	char timeText[SL_MAXIMUM_LOG_TEXT_SIZE];
	ctime_s(timeText, sizeof(char) * SL_MAXIMUM_LOG_TEXT_SIZE, &time);

	for (int i = 0; i < SL_MAXIMUM_LOG_TEXT_SIZE; ++i) {
		if (timeText[i] == '\n') {
			timeText[i] = '\0';
			break;
		}
	}

	std::cout << timeText << " - (L)" << message << '\n';
}

void SlLoggedAudio::LogWithSoundId(const char* message, SlSoundId id)
{
	char text[SL_MAXIMUM_LOG_TEXT_SIZE];
	sprintf_s(text, "%s Id: %d", message, id);
	Log(text);
}
