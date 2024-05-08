#pragma once

#include "object_state.h"

class MonsterState : public ObjectState {
public:
	// 초기화
	virtual void Initialize();

	virtual bool GiveDamage(PERObject& opponent, short physical, short mind);
	virtual bool UseMind(int mind);

	// setter
	void SetSight(double sight);
	void SetWanderDistance(double distance);

	// getter
	double GetSight() const;
	double GetSightSquare() const;
	double GetWanderDistance() const;
	double GetWanderDistanceSquare() const;

private:
	// 기본 시야의 제곱값
	const double c_DEFAULT_SIGHT = 5.0;
	// 기본 배회 거리의 제곱값
	const double c_DEFAULT_WANDER_DISTANCE = 1.0;

	// 시야의 제곱
	double m_sight = c_DEFAULT_SIGHT;
	// 배회 거리의 제곱
	double m_wanderDistance = c_DEFAULT_WANDER_DISTANCE;
};