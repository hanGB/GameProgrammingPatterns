#pragma once
#include "stdafx.h"
#include "command.h"

class InputHandler {
public:
	InputHandler() {};
	~InputHandler() {
		delete m_buttonW;
		delete m_buttonA;
		delete m_buttonS;
		delete m_buttonD;
	};

	void SetButton(Command* w, Command* a, Command* s, Command* d) {
		m_buttonW = w;
		m_buttonA = a;
		m_buttonS = s;
		m_buttonD = d;
	}

	Command* HandleInput() {
		if (GetAsyncKeyState(KEY_INPUT_W)) return m_buttonW;
		if (GetAsyncKeyState(KEY_INPUT_A)) return m_buttonA;
		if (GetAsyncKeyState(KEY_INPUT_S)) return m_buttonS;
		if (GetAsyncKeyState(KEY_INPUT_D)) return m_buttonD;

		return nullptr;
	}

private:
	Command* m_buttonW;
	Command* m_buttonA;
	Command* m_buttonS;
	Command* m_buttonD;
};