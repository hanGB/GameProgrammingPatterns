#pragma once
#include "stdafx.h"
#include "entity.h"

class Observer {
	friend class Subject;

public:
	Observer() : m_next(nullptr), m_prev(nullptr) {}
	virtual ~Observer() {}

	virtual void OnNotify(const Entity& entity, Event event) = 0;

private:
	Observer* m_next;
	Observer* m_prev;
};