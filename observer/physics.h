#pragma once
#include "stdafx.h"
#include "subject.h"

class Physics : public Subject {
public:
	void UpdateEntiy(Entity& entity) {
		bool wasOnSurface = entity.GetIsOnSurface();
		entity.SetAcelerate(0.0f, GRAVITY);
		entity.Update(m_elapsedTime);
		if (wasOnSurface && !entity.GetIsOnSurface()) {
			Notify(entity, Event::EVENT_ENTITY_FELL);
		}
	}

	void SetElapsedTime(float time) {
		m_elapsedTime = time;
	}

private:
	const float GRAVITY = -9.8f;

	float m_elapsedTime;
};