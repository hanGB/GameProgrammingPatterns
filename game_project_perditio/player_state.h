#pragma once

#include "object_state.h"
#include "per_world.h"

class PlayerState : public ObjectState {
public:
	// 초기화
	virtual void Initialize();

	// 이벤트 처리
	virtual void Recive(PEREvent event, PERVec3 data);

	virtual bool GiveDamage(PERObject& opponent, PERWorld& world, PERAudio& audio, short physical, short mind);
	virtual bool UseMind(PERWorld& world, PERAudio& audio, int mind);
	virtual void RecoverPerTime(PERWorld& world, PERAudio& audio, double dTime);
	virtual void GiveExp(PERWorld& world, PERAudio& audio, int exp);
	virtual void KillSelf(PERWorld& world, PERAudio& audio);

	// getter
	double GetShootCoolTime() const;
	std::string& GetBulletVisualId();
	double GetSwingCoolTime() const;
	std::string& GetBladeVisualId();
	PERWorldType GetCurrentWorldType() const;

	void SetCurrentWorldType(PERWorldType type);

	void CopyData(PlayerState* updatedState);
	void MatchDataAndHud();

	const int c_BULLET_XY_FORCE = 1000;
	const int c_BLADE_XY_FORCE = 500;

private:
	const double c_DEFAULT_SHOOT_BULLET_COOL_TIME	= 0.5;
	const double C_DEFAULT_SWING_BLADE_COOL_TIME	= 0.5;
	
	// 공격체 관련
	std::string m_bulletVisualId = "PLAYER_BASIC_BULLET_VISUAL";
	double m_shootCoolTime	= c_DEFAULT_SHOOT_BULLET_COOL_TIME;
	std::string m_bladeVisualId = "PLAYER_BASIC_BLADE_VISUAL";
	double m_swingCoolTime	= C_DEFAULT_SWING_BLADE_COOL_TIME;

	PERWorldType m_currentWorldType = PERWorldType::MAIN_MENU;
};