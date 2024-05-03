#pragma once

#include "object_state.h"

class PlayerState : public ObjectState {
public:
	// 초기화
	virtual void Initialize();

	virtual bool GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);
	virtual void RecoverPerTime(PERObject& object, double dTime);
	virtual void GiveExp(PERObject& object, int exp);

	// getter
	double GetShootCoolTime() const;
	std::string& GetBulletVisualId();
	double GetSwingCoolTime() const;
	std::string& GetBladeVisualId();

	const int c_BULLET_XY_FORCE = 1000;

private:
	const double c_DEFAULT_SHOOT_BULLET_COOL_TIME	= 0.5;
	const double C_DEFAULT_SWING_BLADE_COOL_TIME	= 0.5;
	

	// 공격체 관련
	std::string m_bulletVisualId = "PLAYER_BASIC_BULLET_VISUAL";
	double m_shootCoolTime	= c_DEFAULT_SHOOT_BULLET_COOL_TIME;
	std::string m_bladeVisualId = "PLAYER_BASIC_BLADE_VISUAL";
	double m_swingCoolTime	= C_DEFAULT_SWING_BLADE_COOL_TIME;
};