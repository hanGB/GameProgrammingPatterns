#pragma once
#include "stdafx.h"
#include "command.h"
#include "child_of_command.h"
#include <list>

class CommandList {
public:
	CommandList() { 
		m_headCommad = new NullCommand();
		m_list.push_back(m_headCommad);
		m_currentCommand = m_list.begin();
	}
	~CommandList() { 
		auto it = m_list.begin();
		RemoveCommandsWithStartPoint(it);
	}

	void AddCommand(Command* command) {
		if (m_currentCommand != m_list.end()) {
			auto it = m_currentCommand;
			it++;
			RemoveCommandsWithStartPoint(it);
		}
		m_list.push_back(command);
		m_currentCommand++;
	}

	void Undo() {
		if ((*m_currentCommand) != m_headCommad) {
			std::cout << "undo...\n";
			(*m_currentCommand)->Undo();
			m_currentCommand--;
		}
	}
	void Redo() {
		if (m_currentCommand != m_list.end()) {
			m_currentCommand++;
			if (m_currentCommand != m_list.end()) {
				std::cout << "redo...\n";
				(*m_currentCommand)->Execute();
			}
			else {
				m_currentCommand--;
			}
		}
	}

private:
	void RemoveCommandsWithStartPoint(std::list<Command*>::iterator it) {
		while (it != m_list.end())
		{
			delete (*it);
			m_list.erase(it++);
			continue;
		}
	}

	std::list<Command*> m_list;
	NullCommand* m_headCommad;
	std::list<Command*>::iterator m_currentCommand;
};