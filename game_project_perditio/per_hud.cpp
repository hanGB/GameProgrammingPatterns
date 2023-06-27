#include "stdafx.h"
#include "per_hud.h"
#include "per_renderer.h"

PERHud::PERHud()
{
	m_hp.max = 100;
	m_hp.current = m_hp.max;
	m_hp.showing = m_hp.max;

	m_mp.max = 100;
	m_mp.current = m_mp.max;
	m_mp.showing = m_mp.max;
}

PERHud::~PERHud()
{

}

void PERHud::Update(double dTime)
{
	UpdateShowingValue(m_hp, dTime);
	UpdateShowingValue(m_mp, dTime);
}

void PERHud::Renderer(PERRenderer& renderer)
{
	// ui ���� ������ �Լ� ���� �ʿ�(ȭ�� ��ǥ��� ��ġ ����, PERVec2 ���, ���� ��ġ ����� ���� �޶����� �ʾƾ� ��)
	// hp(�Լ��� ����)
	renderer.RenderShape(PERShapeType::SHAPE_TYPE_RECTANGLE, 
		PERVec3(-4.0, 2.0, 0.0), PERVec3(2.0, 0.4, 5.0), PERColor(255, 255, 255));
	renderer.RenderShape(PERShapeType::SHAPE_TYPE_RECTANGLE, 
		PERVec3(-4.0, 2.0, 0.0), PERVec3(2.0 * m_hp.showing / (double)m_hp.max, 0.4, 0.0), PERColor(255, 0, 0));
	// mp
	renderer.RenderShape(PERShapeType::SHAPE_TYPE_RECTANGLE,
		PERVec3(-4.0, 1.6, 0.0), PERVec3(2.0, 0.4, 5.0), PERColor(255, 255, 255));
	renderer.RenderShape(PERShapeType::SHAPE_TYPE_RECTANGLE,
		PERVec3(-4.0, 1.6, 0.0), PERVec3(2.0 * m_mp.showing / (double)m_mp.max, 0.4, 0.0), PERColor(0, 0, 255));
}

void PERHud::Recive(PEREvent event, PERVec3 data)
{
	switch (event) {
	case PEREvent::EVENT_UPDATE_HP:
		m_hp.current = (int)data.x;
		m_hp.updateSpeed = m_hp.current - m_hp.showing;
		break;
	case PEREvent::EVENT_UPDATE_MP:
		m_mp.current = (int)data.x;
		m_mp.updateSpeed = m_mp.current - m_mp.showing;
		break;
	}
}

void PERHud::UpdateShowingValue(PERUIValue& value, double dTime)
{
	// ���� ���̶� ���̴� ���� ���������� ����
	if (value.showing != value.current) {
		if (value.updateSpeed > 0.0) {
			if (value.current < value.showing) {
				value.showing = value.current;
			}
			else {
				value.showing += value.updateSpeed * 1.0 / c_UPDATE_TIME * dTime;
			}
		}
		else if (value.updateSpeed < 0.0) {
			if (value.current > value.showing) {
				value.showing = value.current;
			}
			else {
				value.showing += value.updateSpeed * 1.0 / c_UPDATE_TIME * dTime;
			}
		}
	}
}
