#pragma once
#include "stdafx.h"
#include "bytecode.h"

class VirtualMachine {
public:
	VirtualMachine() { Init(); }
	~VirtualMachine() { }

	void Interpret(char bytecode[], int size)
	{
		for (int i = 0; i < size; ++i) {
			char instruction = bytecode[i];
			switch (instruction) 
			{
			case INST_LITERAL: {
				// 바이트코드에서 다음 바이트 값을 읽어 stack에 넣음
				int value = bytecode[++i];
				Push(value);
				break;
			}

			case INST_SET_HEALTH: {
				int amount = Pop();
				int wizard = Pop();
				SetHealth(wizard, amount);
				break;
			}
			case INST_SET_WISDOM: {
				int amount = Pop();
				int wizard = Pop();
				SetWisdom(wizard, amount);
				break;
			}

			case INST_SET_AGILITY: {
				int amount = Pop();
				int wizard = Pop();
				SetAgility(wizard, amount);
				break;
			}

			case INST_PLAY_SOUND: {
				int id = Pop();
				PlaySound(id);
				break;
			}

			case INST_PLAY_PARTICLES: {
				int type = Pop();
				SpawnParticles(type);
				break;
			}

			}
		}
	}

private:
	void Init()
	{
		while (!m_stack.empty()) m_stack.pop();
	}

	void Push(int value) 
	{
		m_stack.push(value);
	}
	int Pop()
	{
		// 스택이 비어있지 않는지 확인
		assert(!m_stack.empty());

		int value = m_stack.top();
		m_stack.pop();
		return value;
	}

	void SetHealth(int wizard, int amount)
	{
		if (wizard == 0) {
			std::cout << "Player: ";
		}
		else {
			std::cout << "Enemy: ";
		}
		std::cout << "Health + " << amount << '\n';
	}
	void SetWisdom(int wizard, int amount)
	{
		if (wizard == 0) {
			std::cout << "Player: ";
		}
		else {
			std::cout << "Enemy: ";
		}
		std::cout << "Wisdom + " << amount << '\n';
	}
	void SetAgility(int wizard, int amount)
	{
		if (wizard == 0) {
			std::cout << "Player: ";
		}
		else {
			std::cout << "Enemy: ";
		}
		std::cout << "Agility + " << amount << '\n';
	}

	void PlaySound(int soundId)
	{
		std::cout << "Play Sound: ID " << soundId << '\n';
	}
	void SpawnParticles(int particleType)
	{
		std::cout << "Spawn Particles: Type " << particleType << '\n';
	}

	std::stack<int> m_stack;
};