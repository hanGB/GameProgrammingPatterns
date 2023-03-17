#pragma once
#include "stdafx.h"
#include "bytecode.h"
#include "wizard.h"

class VirtualMachine {
public:
	VirtualMachine() 
	{ 
		m_wizard[0] = new Wizard("Player", 100, 10, 5);
		m_wizard[1] = new Wizard("Enemy", 50, 8, 4);

		Init(); 
	}
	~VirtualMachine() 
	{ 
		for (int i = 0; i < MAX_WIZARD; ++i) {
			delete m_wizard[i];
		}
	}

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

			case INST_GET_HEALTH: {
				int wizard = Pop();
				Push(GetHealth(wizard));
				break;
			}
			case INST_GET_WISDOM: {
				int wizard = Pop();
				Push(GetWisdom(wizard));
				break;
			}
			case INST_GET_AGILITY: {
				int wizard = Pop();
				Push(GetAgility(wizard));
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

			case INST_ADD: {
				int b = Pop();
				int a = Pop();
				std::cout << a << " + " << b << '\n';
				Push(a + b);
				break;
			}
			case INST_SUB: {
				int b = Pop();
				int a = Pop();
				std::cout << a << " - " << b << '\n';
				Push(a - b);
				break;
			}
			case INST_MULTI: {
				int b = Pop();
				int a = Pop();
				std::cout << a << " * " << b << '\n';
				Push(a * b);
				break;
			}
			case INST_DIVI: {
				int b = Pop();
				int a = Pop();
				std::cout << a << " / " << b << '\n';
				Push(a / b);
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
		// 스택이 최대 크기보다 작은지 확인
		assert((m_stack.size() <= MAX_STACK_SIZE));

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
		if (wizard == PLAYER_ID) m_wizard[PLAYER_ID]->SetHeallth(amount);
		else m_wizard[1]->SetHeallth(amount);
	}
	void SetWisdom(int wizard, int amount)
	{
		if (wizard == PLAYER_ID) m_wizard[PLAYER_ID]->SetWisdom(amount);
		else m_wizard[1]->SetWisdom(amount);
	}
	void SetAgility(int wizard, int amount)
	{
		if (wizard == PLAYER_ID) m_wizard[PLAYER_ID]->SetAgility(amount);
		else m_wizard[1]->SetAgility(amount);
	}
	int GetHealth(int wizard)
	{
		if (wizard == PLAYER_ID) return m_wizard[PLAYER_ID]->GetHealth();
		else return m_wizard[1]->GetHealth();
	}
	int GetWisdom(int wizard)
	{
		if (wizard == PLAYER_ID) return m_wizard[PLAYER_ID]->GetWisdom();
		else return m_wizard[1]->GetWisdom();
	}
	int GetAgility(int wizard)
	{
		if (wizard == PLAYER_ID) return m_wizard[PLAYER_ID]->GetAgility();
		else return m_wizard[1]->GetAgility();
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
	Wizard* m_wizard[2];
};