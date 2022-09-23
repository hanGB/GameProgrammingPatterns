#pragma once
#include "stdafx.h"
#include "child_of_command.h"
#include "game_actor.h"

class InputHandler {
public:
	InputHandler() {};
	~InputHandler() {};

	void SetUint(Unit& uint) {
		m_unit = &uint;
	}

	Command* HandleInput() {

		if (GetAsyncKeyState(VK_UP) & 0x0001) {
			int desY = m_unit->GetY() - 1;

			Command* command = new MoveUnitCommand(m_unit, m_unit->GetX(), desY);
			return command;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001) {
			int desY = m_unit->GetY() + 1;

			Command* command = new MoveUnitCommand(m_unit, m_unit->GetX(), desY);
			return command;
		}
		return nullptr;
	}

private:
	Unit* m_unit;
};