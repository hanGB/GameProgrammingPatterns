#pragma once
#include "stdafx.h"
#include "observer.h"
#include "entity.h"

class Achievements : public Observer {
public:
	virtual void OnNotify(const Entity& entity, Event event) override {
		switch (event)
		{
		case Event::EVENT_ENTITY_FELL:
			if (entity.GetIsHero()) {
				unlock(Achievement::ACHIEVEMENT_FELL);
			}
			break;
		case Event::EVENT_ENTITY_DEATH:
			if (entity.GetIsHero()) {
				lock(Achievement::ACHIEVEMENT_NO_DEATH);
			}
			break;
		default:
			break;
		}
	}

private:
	void unlock(Achievement achievement) {
		switch (achievement)
		{
		case Achievement::ACHIEVEMENT_FELL:
			if (!m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_FELL)]) {
				std::cout << "Hero is falling down!!!\n";
				m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_FELL)] = true;
			}
			break;
		case Achievement::ACHIEVEMENT_NO_DEATH:
			if (!m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_NO_DEATH)]) {
				std::cout << "Unknown death\n";
				m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_NO_DEATH)] = true;
			}
			break;
		default:
			break;
		}
	}
	void lock(Achievement achievement) {
		switch (achievement)
		{
		case Achievement::ACHIEVEMENT_NO_DEATH:
			if (!m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_NO_DEATH)]) {
				m_isAchieved[static_cast<int>(Achievement::ACHIEVEMENT_NO_DEATH)] = true;
			}
			break;
		default:
			break;
		}
	}

	bool m_isAchieved[static_cast<int>(Achievement::SIZE)] = { false, };
};