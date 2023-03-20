#pragma once
#include "stdafx.h"
#include "sound_player.h"
#include "particle_spawner.h"
#include "object.h"

class Superpower {
public:
	virtual ~Superpower() {}

	void UsePower() {
		Activate();
	}
	static void Init(SoundPlayer* player, ParticleSpawner* spawner) {
		m_soundPlayer = player;
		m_particleSpawner = spawner;
	}
	void SetObject(Object* object) {
		m_object = object;
	}

protected:
	virtual void Activate() = 0;

	SoundPlayer& GetSoundPlayer() {
		return *m_soundPlayer;
	}

	double GetObjectX() const {
		double x, temp;
		m_object->GetPosition(&x, &temp, &temp);
		return x;
	}
	double GetObjectY() const {
		double y, temp;
		m_object->GetPosition(&temp, &y, &temp);
		return y;
	}
	double GetObjectZ() const {
		double z, temp;
		m_object->GetPosition(&temp, &temp, &z);
		return z;
	}

	void Move(double x, double y, double z) {
		m_object->SetPosition(x, y, z);
	}
	void PlaySound(SoundID id, double volume) {
		GetSoundPlayer().PlaySound(id, volume);
	}
	void SpawnParticles(ParticleType type, int count) {
		m_particleSpawner->SpawnParticle(type, count);
	}

private:
	static SoundPlayer* m_soundPlayer;
	static ParticleSpawner* m_particleSpawner;

	Object* m_object;
};

// class 안에 있는 static 변수 재선언
SoundPlayer* Superpower::m_soundPlayer;
ParticleSpawner* Superpower::m_particleSpawner;
