#pragma once
/*
*	���� ������Ʈ ����(����)�� ��� Ŭ����
* 
*	������Ʈ�� �����ϵ��� ����
*/

class ObjectState {
public:
	ObjectState();
	~ObjectState();

	PERStat GetStat() const;
	int GetCollisionDamage() const;

	void SetStat(PERStat stat);
	void SetIsHasCollisionDamage(bool damage);
	void SetIsImmortal(bool immortal);
	
	// ���� ��ȭ
	void GiveDamage(short physical, short mind);
	bool UseMind(int mind);

private:
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;

	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;
};