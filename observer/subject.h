#pragma once
#include "stdafx.h"
#include "observer.h"

class Subject {

public:
	Subject() : m_head(nullptr) {}

	void AddObserver(Observer* observer) {
		observer->m_next = m_head;
		m_head = observer;
	}
	void RemoveObserver(Observer* observer) {
		if (m_head == observer) {
			m_head = observer->m_next;
			observer->m_next = nullptr;
			return;
		}

		auto current = m_head;
		while (current != nullptr) {
			if (current->m_next == observer) {
				current->m_next = observer->m_next;
				observer->m_next = nullptr;
				return;
			}
			current = current->m_next;
		}
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