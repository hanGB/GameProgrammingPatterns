#pragma once
/*
*	���� ������Ʈ ����(����)�� ��� Ŭ����
* 
*	������Ʈ�� �����ϵ��� ����
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
	
	// ���� ��ȭ
	void GiveDamage(PERObject& object, short physical, short mind);
	bool UseMind(PERObject& object, int mind);

private:
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;

	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;
};