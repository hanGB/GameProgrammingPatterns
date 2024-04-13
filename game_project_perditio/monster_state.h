#pragma once

#include "object_state.h"

class MonsterState : public ObjectState {
public:
	virtual bool GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);
	virtual void MatchFloatingUI(PERObject& object);

	virtual void ShowFloatingUi(PERObject& object, PERHud* hud);

	// setter
	void SetSight(double sight);
	void SetWanderDistance(double distance);

	// getter
	double GetSight() const;
	double GetSightSquare() const;
	double GetWanderDistance() const;
	double GetWanderDistanceSquare() const;

private:
	// �⺻ �þ��� ������
	const double c_DEFAULT_SIGHT = 5.0;
	// �⺻ ��ȸ �Ÿ��� ������
	const double c_DEFAULT_WANDER_DISTANCE = 1.0;

	// �þ��� ����
	double m_sight = c_DEFAULT_SIGHT;
	// ��ȸ �Ÿ��� ����
	double m_wanderDistance = c_DEFAULT_WANDER_DISTANCE;
};