#pragma once
#include "per_audio.h"

class IRRAudio : public PERAudio {
public:
	IRRAudio();
	~IRRAudio();

	virtual void Update();

	virtual void LoadSound(PERSoundId id, const char* fileName);

private:
	// 일회성 소리(효과음)
	virtual void PlaySoundOneTime(PERSoundId id, double volume);
	virtual void StopAllSounds();

	// 배경음악
	virtual void SetBGM(PERSoundId id, double volume);
	virtual void PlayBGM();
	virtual void StopBGM();
	virtual void PauseBGM(bool isPause);

	// 환경음
	virtual void SetAmbientSound(PERSoundId id, double volume, int slot);
	virtual void PlayAmbientSound(int slot);
	virtual void StopAmbientSound(int slot);
	virtual void PauseAmbientSound(bool isPause, int slot);
	virtual void PlayAllAmbientSounds();
	virtual void StopAllAmbientSounds();
	virtual void PauseAllAmbientSounds(bool isPause);

	irrklang::ISoundEngine* m_engine;

	std::map<PERSoundId, irrklang::ISoundSource*> m_soundMap;

	irrklang::ISound* m_backgroundMusic;
	irrklang::ISound* m_ambientSounds[PER_MAXIMUM_AMBIEMNT_SOUND_SLOT];
};