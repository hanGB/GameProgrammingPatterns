#pragma once
#include "stdafx.h"
#include "command.h"
#include "child_of_command.h"

class AiController{
public:
	AiController() {
		m_lastTime = std::clock();
	}
	~AiController() {

	};

	Command* GiveCommand() {
		m_currentTime = std::clock();
		double delaTime = double(m_currentTime - m_lastTime) / CLOCKS_PER_SEC;
		m_timer += delaTime;
		m_lastTime = m_currentTime;

		if (m_timer < 0.5f) return nullptr;
		m_timer = 0.0f;

		std::uniform_int_distribution<int> uid(0, 4);
		int randomValue = uid(m_dre);

		if (randomValue == 0) return new FireCommand();
		if (randomValue == 1) return new JumpCommand();
		if (randomValue == 2) return new ReloadCommand();
		
		return new NullCommand();
	}

private:
	std::default_random_engine m_dre;
	std::clock_t m_lastTime, m_currentTime;
	float m_timer = 0.0f;
};