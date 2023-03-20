#pragma once
#include "stdafx.h"
#include "superpower.h"

class SkyLaunch : public Superpower {
protected:
	virtual void Activate() {
		if (GetObjectZ() == 0.0) {
			PlaySound(SoundID::SOUND_SPROING, 1.0);
			SpawnParticles(ParticleType::PARITCLE_DUST, 10);
			Move(0.0, 0.0, 20.0);
		}
		else if (GetObjectZ() < 10.0f) {
			PlaySound(SoundID::SOUND_SWOOP, 1.0f);
			Move(0.0, 0.0, GetObjectZ() + 20.0);
		}
		else {
			PlaySound(SoundID::SOUND_DIVE, 1.0);
			SpawnParticles(ParticleType::PARTICLE_SPARKLES, 10);
			Move(0.0, 0.0, -GetObjectZ());
		}
	}
};
