#include "stdafx.h"
#include "per_particle.h"
#include "per_renderer.h"

PERParticle::PERParticle()
{
	m_next = nullptr;
	m_isInUse = false;
}

PERParticle::~PERParticle()
{
}

void PERParticle::Init(PERShapeType type, PERVec3 pos, PERVec3 size, double mass, PERVec3 force, PERVec3 vel, PERColor color,
	double lifeTime, bool isBorderOn, int borderWidth, PERColor borderColor)
{
	m_isInUse = true;
	m_lifeTime = lifeTime;

	// ������
	m_position = pos;
	m_size = size;
	m_force = force;
	m_velocity = vel;
	m_mass = mass;

	// ����
	m_shapeType = type;
	m_color = color;
	m_isBorderOn = isBorderOn;
	m_borderWidth = borderWidth;
	m_borderColor = borderColor;
}

bool PERParticle::Update(double dTime)
{
	if (!GetIsInUse()) return true;
	m_lifeTime -= dTime;
	// ������ Ÿ���� 0���� ������ ��� ���� �ƴϵ��� �ϱ� ���� false�� �ѱ�
	if (m_lifeTime < 0.0) return false;

	// ���� ���� �ӵ� ��ȭ ���
	PERVec3 acc = m_force * (1.0 / m_mass);
	m_velocity = m_velocity + acc * dTime;
	m_position = m_position + m_velocity * dTime + acc * 0.5 * dTime * dTime;

	m_force = PERVec3(0, 0, 0);
	return true;
}

void PERParticle::Render(PERRenderer& renderer)
{
	if (!GetIsInUse()) return;

	renderer.RenderShapeInWorldCoordinate(
		m_shapeType, m_position, m_size, m_color, m_isBorderOn, m_borderWidth, m_borderColor);
}

bool PERParticle::GetIsInUse() const
{
	return m_isInUse;
}

void PERParticle::SetIsInUse(bool use)
{
	m_isInUse = use;
}

PERParticle* PERParticle::GetNext()
{
	return m_next;
}

void PERParticle::SetNext(PERParticle* next)
{
	m_next = next;
}
