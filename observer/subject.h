#pragma once
#include "stdafx.h"
#include "observer.h"

class Subject {
public:
	Subject() : m_head(nullptr) { }

	void AddObserver(Observer* observer) {
		observer->m_next = m_head;
		if (m_head != nullptr) m_head->m_prev = observer;
		observer->m_prev = nullptr;
		m_head = observer;
	}
	void RemoveObserver(Observer* observer) {
		if (m_head == observer) {
			m_head = observer->m_next;
		}
		if (observer->m_next != nullptr) {
			observer->m_next->m_prev = observer->m_prev;
		}
		if (observer->m_prev != nullptr) {
			observer->m_prev->m_next = observer->m_next;
		}
		observer->m_next = nullptr;
		observer->m_prev = nullptr;
	}

protected:
	void Notify(const Entity& entity, Event event) {
		auto observer = m_head;
		while (observer != nullptr) {
			observer->OnNotify(entity, event);
			observer = observer->m_next;
		}
	}

private:
	Observer* m_head;

};