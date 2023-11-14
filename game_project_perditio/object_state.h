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
	PERVec3 GetSpawnPosition() const;

	void SetStat(PERStat stat);
	void SetIsHasCollisionDamage(bool damage);
	void SetIsImmortal(bool immortal);
	void SetSpawnPosition(PERVec3 position);
	
	// ���� ��ȭ
	virtual bool GiveDamage(PERObject& object, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);

protected:
	// ����
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;

	// �⺻ ����
	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;

	// ���� ��ġ
	PERVec3 m_spawnPosition = PERVec3(0.0, 0.0, 0.0);
};