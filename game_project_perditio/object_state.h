#pragma once
/*
*	현재 오브젝트 상태(정보)를 담는 클래스
* 
*	오브젝트가 소유하도록 구현
*/

class PERObject;

class ObjectState {
public:
	ObjectState();
	~ObjectState();

	PERStat GetStat() const;
	int GetCollisionDamage() const;
	int GetCurrentBody() const;
	int GetCurrentMind() const;
	PERVec3 GetSpawnPosition() const;

	void SetStat(PERStat stat);
	void SetIsHasCollisionDamage(bool damage);
	void SetIsImmortal(bool immortal);
	void SetSpawnPosition(PERVec3 position);
	
	// 상태 변화
	virtual bool GiveDamage(PERObject& object, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);

protected:
	// 스탯
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;

	// 기본 정보
	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;

	// 스폰 위치
	PERVec3 m_spawnPosition = PERVec3(0.0, 0.0, 0.0);
};