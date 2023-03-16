#pragma once
#include "stdafx.h"
#include "bytecode.h"

class VirtualMachine {
public:
	void Interpret(char bytecode[], int size)
	{
		for (int i = 0; i < size; ++i) {
			char instruction = bytecode[i];
			switch (instruction) 
			{

			case INST_SET_HEALTH:
				SetHealth(0, 100);
				break;

			case INST_SET_WISDOM:
				SetWisdom(0, 100);
				break;

			case INST_SET_AGILITY:
				SetAgility(0, 100);
				break;

			case INST_PLAY_SOUND:
				PlaySound(Sound::SOUND_BANG);
				break;

			case INST_PLAY_PARTICLES:
				SpawnParticles(Particles::PARTICLE_FLAME);
				break;

			}
		}
	}

private:
	void SetHealth(int wizard, int amount)
	{
		std::cout << "set health\n";
	}
	void SetWisdom(int wizard, int amount)
	{
		std::cout << "set wisdom\n";
	}
	void SetAgility(int wizard, int amount)
	{
		std::cout << "set agility\n";
	}

	void PlaySound(int soundId)
	{
		std::cout << "play sound\n";
	}
	void SpawnParticles(int particleType)
	{
		std::cout << "spawn particles\n";
	}
};