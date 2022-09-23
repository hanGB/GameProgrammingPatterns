#pragma once
#include "stdafx.h"
#include "command.h"
#include "game_actor.h"

class MoveUnitCommand : public Command {
public:
	MoveUnitCommand(Unit* unit, int x, int y) : m_unit(unit), m_x(x), m_y(y), m_beforeX(0), m_beforeY(0) {}

	virtual void Execute() {
		m_beforeX = m_unit->GetX();
		m_beforeY = m_unit->GetY();
		m_unit->MoveTo(m_x, m_y);
	}

	virtual void Undo() {
		m_unit->MoveTo(m_beforeX, m_beforeY);
	}

private:
	Unit* m_unit;
	int m_x, m_y;
	int m_beforeX, m_beforeY;
};

class NullCommand : public Command {
public:
	NullCommand() {}

	virtual void Execute() {
		
	}
	virtual void Undo() {
		
	}
};