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

	void SetStat(PERStat stat);
	void SetIsHasCollisionDamage(bool damage);
	void SetIsImmortal(bool immortal);
	
	// 상태 변화
	void GiveDamage(PERObject& object, short physical, short mind);
	bool UseMind(PERObject& object, int mind);

private:
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;

	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;
};