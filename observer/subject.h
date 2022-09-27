#pragma once
#include "stdafx.h"
#include "observer.h"

class Subject {
public:
	void AddObserver(Observer* observer) {
		m_observers.push_back(observer);
	}
	void RemoveObserver(Observer* observer) {
		for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
			if ((*it) == observer) {
				delete *it;
				m_observers.erase(it++);
				break;
			}
		}
	}
	
protected:
	void Notify(const Entity& entity, Event event) {
		for (auto observer : m_observers) {
			observer->OnNotify(entity, event);
		}
	}

private:
	std::list<Observer*> m_observers;
};