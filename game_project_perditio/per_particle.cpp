#include "stdafx.h"
#include "per_particle.h"
#include "per_renderer.h"
#include "black_board.h"
#include "event_dispatcher.h"

PERParticle::PERParticle()
{
	m_next = nullptr;
	m_isInUse = false;
}

PERParticle::~PERParticle()
{
}

void PERParticle::Init(PERShapeType type, PERVec3 pos, PERVec3 size, double mass, PERVec3 force, PERVec3 vel, PERColor color, 
	double lifeTime, bool isColletedByPlayer, bool isBorderOn, int borderWidth, PERColor borderColor)
{
	m_isInUse = true;
	m_lifeTime = lifeTime;
	m_halfSettingLifeTime = m_lifeTime * 0.5;

	// 물리값
	m_position = pos;
	m_size = size;
	m_force = force;
	m_velocity = vel;
	m_mass = mass;

	// 외형
	m_shapeType = type;
	m_color = color;
	m_isBorderOn = isBorderOn;
	m_borderWidth = borderWidth;
	m_borderColor = borderColor;

	// 움직임 함수 결정
	if ( isColletedByPlayer ) m_Move = &PERParticle::MoveConsideringPlayer;
	else m_Move = &PERParticle::MoveNormally;

}

bool PERParticle::Update(double dTime)
{
	if (!GetIsInUse()) return true;
	m_lifeTime -= dTime;
	// 라이프 타임이 0보다 작으면 사용 중이 아니도록 하기 위해 false를 넘김
	if (m_lifeTime < 0.0) return false;

	return m_Move(*this, dTime);
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

bool PERParticle::MoveNormally(double dTime)
{
	// 힘에 의한 속도 변화 계산
	PERVec3 acc = m_force * ( 1.0 / m_mass );
	m_velocity = m_velocity + acc * dTime;
	m_position = m_position + m_velocity * dTime + acc * 0.5 * dTime * dTime;

	m_force = PERVec3(0, 0, 0);

	return true;
}

bool PERParticle::MoveConsideringPlayer(double dTime)
{
	// 라이프 타임이 절반 남았을 경우부터 플레이어가 흡수 가능
	if ( m_lifeTime < m_halfSettingLifeTime )
	{
		PERVec3 pos = BlackBoard::GetPlayerPos();
		double distance = DistanceSquareAandBIgnoringZValue(pos, m_position);
		// 특정 거리 안이면 플레이어를 향해 이동
		if ( c_COLLECT_DISTANCE_2 > distance && BlackBoard::GetIsPlayerLiving() )
		{
			// 충분히 가까워 졌을 경우 삭제
			if ( c_EATEN_DISTANCE_2 > distance )
			{
				EventDispatcher::Send(PEREvent::ADD_BD_TO_PLAYER, PERVec3(1.0, 0.0, 0.0));
				return false;
			}
			double rDistance = std::sqrt(distance);
			m_velocity = PERVec3(( pos.x - m_position.x ) / rDistance, ( pos.y - m_position.y ) / rDistance, 0.0) * c_COLLETED_SPEED;
		}
	}

	return MoveNormally(dTime);
}
