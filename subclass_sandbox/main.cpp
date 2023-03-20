#include "stdafx.h"
#include "sound_player.h"
#include "particle_spawner.h"
#include "hero.h"

int main()
{
	ParticleSpawner* particleSpawner = new ParticleSpawner();
	SoundPlayer* soundPlayer = new SoundPlayer();

	Hero* hero = new Hero(soundPlayer, particleSpawner);

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
