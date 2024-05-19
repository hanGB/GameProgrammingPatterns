#include "respawnable_player_state.h"
#include "event_dispatcher.h"
#include "per_object.h"
#include "black_board.h"
void RespawnablePlayerState::Initialize()
{
	m_respawnTime = c_RESPAWN_DELAY;
	m_isHaveToRespawn = false;

	PlayerState::Initialize();
}

void RespawnablePlayerState::RecoverPerTime(PERWorld& world, PERAudio& audio, double dTime)
{
	PlayerState::RecoverPerTime(world, audio, dTime);

	if (!m_isHaveToRespawn) return;
	m_respawnTime -= dTime;
	if (m_respawnTime < 0.0) {
		EventDispatcher::Send(PEREvent::GO_TO_RESPAWN, PERVec3(0.0, 0.0, 0.0));
		// 다시 이벤트를 보내지 않도록 절대 끝나지 않는 값 설정
		m_respawnTime = PER_MAXIMUM_LIFE_TIME;
	}
}

void RespawnablePlayerState::KillSelf(PERWorld& world, PERAudio& audio)
{
	if (m_isHaveToRespawn) return;
	
	// 주인공 죽은 걸로 설정
	BlackBoard::SetIsPlayerLiving(false);

	PERStat stat;
	// 레벨을 이펙트 타입으로 사용
	stat.level = (short)PERParticleEffectType::CIRCLE_BOMB;
	// 바디를 흡수 여부로 사용
	stat.body = (short)false;

	world.RequestAddObject(GetOwner(), PERObjectType::PARTICLE_EFFECTER, "PARTICLE_EFFECT_PLAYPER_DEATH_VISUAL", PERDatabaseType::EFFECT,
		stat, GetOwner()->GetPosition(), 1.0, PERVec3(0.0, 0.0, 0.0));

	GetOwner()->GetGraphics().RemoveFloatingUi();

	GetOwner()->SetSize(PERVec3(0.0, 0.0, 0.0));
	m_isHaveToRespawn = true;
}
