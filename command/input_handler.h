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

	void HandleInput() {
		if (GetAsyncKeyState(KEY_INPUT_W)) m_buttonW->Execute();
		else if (GetAsyncKeyState(KEY_INPUT_A)) m_buttonA->Execute();
		else if (GetAsyncKeyState(KEY_INPUT_S)) m_buttonS->Execute();
		else if (GetAsyncKeyState(KEY_INPUT_D)) m_buttonD->Execute();
	}

private:
	Command* m_buttonW;
	Command* m_buttonA;
	Command* m_buttonS;
	Command* m_buttonD;
};