#pragma once
#include "per_audio.h"

class IRRAudio : public PERAudio {
public:
	IRRAudio();
	~IRRAudio();

	virtual void Update();

	virtual void LoadSound(PERSoundId id, const char* fileName);

private:
	virtual void PlaySound(PERSoundId id, double volume);
	virtual void StopSound(PERSoundId id);
	virtual void StopAllSounds();

	virtual void SetBGM(PERSoundId id, double volume);
	virtual void AddBGM(PERSoundId id, double volume);
	virtual void PlayCurrentBGM();
	virtual void StopCurrentBGM();
	virtual void PauseCurrentBGM(bool isPause);
	virtual void StopAllBGMs();


	irrklang::ISoundEngine* m_engine;

	std::map<PERSoundId, irrklang::ISoundSource*> m_soundMap;

	irrklang::ISound* m_bgm = nullptr;
};