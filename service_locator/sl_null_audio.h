#pragma once
#include "stdafx.h"
#include "sl_audio.h"

class SlNullAudio : public SlAudio {
	virtual void PlaySound(SlSoundId id)	{ /* Do Nothing */ };
	virtual void StopSound(SlSoundId id)	{ /* Do Nothing */ };
	virtual void StopAllSounds()			{ /* Do Nothing */ };
};