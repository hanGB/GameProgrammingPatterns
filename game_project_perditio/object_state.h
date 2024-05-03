#pragma once
/*
*	���� ������Ʈ ����(����)�� ��� Ŭ����
* 
*	������Ʈ�� �����ϵ��� ����
*/

class PERObject;
class PERHud;
class UiElement;

class ObjectState {
public:
	ObjectState();
	~ObjectState();

	// ����� ����
	void UseIgnoreDamageTime(double dTime);

	// �ʱ�ȭ
	virtual void Initialize();

	// ���� ��ȭ
	virtual bool GiveDamage(PERObject& object, PERObject& opponent, short physical, short mind);
	virtual bool UseMind(PERObject& object, int mind);
	virtual void RecoverPerTime(PERObject& object, double dTime);
	virtual void GiveExp(PERObject& object, int exp);

	std::string GetNameId() const;
	PERStat GetStat() const;
	int GetCollisionDamage() const;
	int GetCurrentBody() const;
	int GetCurrentMind() const;
	PERVec3 GetSpawnPosition() const;

	void SetNameId(std::string& nameId);
	void SetNameId(const char* name);
	void SetStat(PERStat stat);
	void SetIsHasCollisionDamage(bool damage);
	void SetIsImmortal(bool immortal);
	void SetSpawnPosition(PERVec3 position);

protected:
	static const int	c_DEFAULT_MAXIMUM_FLOATING_UI = 5;

	const double c_DEFAULT_TIME_FOR_RECOVER = 3.0;
	const double c_DEFAULT_BODY_RECOVER_PERCENT = 0.025;
	const double c_DEFAULT_MIND_RECOVER_PERCENT = 0.1;
	const double c_DEFAULT_IGNORE_DAMAGE_TIME = 0.1;
	const int	 c_DEFAULT_LEVEL_EXP_GAP = 2;

	// ����
	std::string m_nameId;
	PERStat m_stat;
	int m_currentBody;
	int m_currentMind;
	int m_exp;

	// ����� ����
	double m_damageDelay = 0.0;
	// �ڵ� ȸ��
	double m_recoverDelay = 0.0;
	double m_recoverTime = c_DEFAULT_TIME_FOR_RECOVER;
	double m_bodyRecoverPercent = c_DEFAULT_BODY_RECOVER_PERCENT;
	double m_mindRecoverPercent = c_DEFAULT_MIND_RECOVER_PERCENT;

	// �⺻ ����
	bool m_isHasCollsionDamage = false;
	bool m_isImmortal = false;

	// ���� ��ġ
	PERVec3 m_spawnPosition = PERVec3(0.0, 0.0, 0.0);
};