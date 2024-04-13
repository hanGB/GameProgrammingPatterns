#pragma once

#include "object_state.h"

class PlayerState : public ObjectState {
public:
	virtual bool GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);
	virtual void RecoverPerTime(PERObject& object, double dTime);
	virtual void GiveExp(PERObject& object, int exp);

	// getter
	double GetShootCoolTime() const;
	PERVec3 GetBulletSize() const;
	double GetSwingCoolTime() const;
	PERVec3 GetBladeSize() const;

	const int c_BULLET_XY_FORCE = 1000;

private:
	const double c_DEFAULT_SHOOT_BULLET_COOL_TIME	= 0.5;
	const PERVec3 C_DEFAULT_BULLET_SIZE				= PERVec3(0.2, 0.2, 0.0);
	const double C_DEFAULT_SWING_BLADE_COOL_TIME	= 0.5;
	const PERVec3 C_DEFAULT_BLADE_SIZE				= PERVec3(0.7, 0.7, 0.0);
	

	// 공격체 관련
	double m_shootCoolTime	= c_DEFAULT_SHOOT_BULLET_COOL_TIME;
	PERVec3 m_bulletSize	= C_DEFAULT_BULLET_SIZE;
	double m_swingCoolTime	= C_DEFAULT_SWING_BLADE_COOL_TIME;
	PERVec3 m_bladeSize		= C_DEFAULT_BLADE_SIZE;

};