#include "stdafx.h"
#include "sound_player.h"
#include "particle_spawner.h"
#include "hero.h"
#include "superpower.h"

int main()
{
	SoundPlayer* soundPlayer = new SoundPlayer();
	ParticleSpawner* particleSpawner = new ParticleSpawner();
	
	Superpower::Init(soundPlayer, particleSpawner);

	Hero* hero = new Hero();

	int count = 0;
	while (count < 100) {
		hero->Update();
		hero->Render();
		count++;
	}

	delete hero;
	delete soundPlayer;
	delete particleSpawner;
}
