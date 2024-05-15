#include "stdafx.h"
#include "test_world.h"
#include "spawner_ai_component.h"
#include "per_object.h"
#include "object_storage.h"
#include "game_mode.h"
#include "response_to_signal_ai_component.h"
#include "making_signal_ai_component.h"
#include "creating_particles_ai_component.h"
#include "test_game_mode.h"
#include "event_dispatcher.h"
#include "player_state.h"
#include "per_audio.h"

TestWorld::TestWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	InitSettingForWorld(objectStorage, database, new TestGameMode());
}

TestWorld::~TestWorld()
{
	
}

void TestWorld::Enter(PERAudio& audio)
{
	PERWorldType type = m_gameMode->GetPlayerState().GetCurrentWorldType();
	
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 0.0, 0.1));
	if (type == PERWorldType::TEST_WORLD2) m_gameMode->GetPlayer().SetPosition(PERVec3(11.0, 0.0, 0.1));

	m_gameMode->GetPlayerState().SetCurrentWorldType(PERWorldType::TEST_WORLD);
	m_gameMode->GetPlayer().SetCurrentPositionToSpawnPosition();

	audio.RequestHandleSound(PERAudioMessageId::SET_BGM, PERSoundId::BATTLE_BGM, 0.75);

	PERWorld::Enter(audio);
}

void TestWorld::Exit(PERAudio& audio)
{
	PERWorld::Exit(audio);
}

void TestWorld::Pause(PERAudio& audio)
{
	m_gameMode->GetPlayerState().SetCurrentWorldType(PERWorldType::TEST_WORLD);
	m_playerPosBeforePause = m_gameMode->GetPlayer().GetPosition();
	PERWorld::Pause(audio);
}

void TestWorld::Resume(PERAudio& audio)
{
	m_gameMode->GetPlayer().SetPosition(m_playerPosBeforePause);
	PERWorld::Resume(audio);
}

void TestWorld::AddFixedAndPhysicalObjects()
{
	// 플랫폼
	PERObject* platform;
	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	platform->SetPosition(PERVec3(0.0, 0.0, -1.0));
	platform->SetSize(PERVec3(15.0, 20.0, 1.0));
	SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
	AddObject(platform);

	for (double x = 0.0; x <= 20.0; x += 20.0) {
		platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
		platform->SetPosition(PERVec3(-10.0 + x, 0.0, -1.0));
		platform->SetSize(PERVec3(5.0, 5.0, 1.0));
		SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
		AddObject(platform);
	}

	// 벽
	PERObject* wall;
	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-1.0, 0.0, 1.0));
	wall->SetSize(PERVec3(1.0, 2.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 1.0, 1.1));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -1.0, 1.1));
	wall->SetSize(PERVec3(2.0, 1.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(100, 125, 150));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-5.0, 0.0, 0.1));
	wall->SetSize(PERVec3(0.5, 5.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(5.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);
}

void TestWorld::AddOtherObjects()
{
	// 몬스터 생성기
	for (double x = -3.0; x <= 3.0; x += 6.0) {
		for (double y = -3.0; y <= 3.0; y += 6.0) {
			PERObject* monsterSpanwer;
			monsterSpanwer = m_objectStorage->PopObject(PERObjectType::SPAWNER);
			monsterSpanwer->SetPosition(PERVec3(x, y, 0.1));
			dynamic_cast<SpawnerAiComponent*>(&monsterSpanwer->GetAi())->SetSpawner(
			"MONSTER_KOPPER", PERObjectType::MONSTER, PERSpawnType::LIVE
			);
			AddObject(monsterSpanwer);
		}
	}

	// 움직이는 벽돌(충돌 처리 문제 해결 불가능시 제거)
	PERObject* block;
	/*block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(3.0, 3.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);
	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-3.0, -3.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);
	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(3.0, -3.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);
	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-3.0, 3.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);
	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-3.0, 0.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);*/

	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(7.0, 3.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	// 문
	PERObject* door;
	door = m_objectStorage->PopObject(PERObjectType::DOOR);
	door->SetPosition(PERVec3(9.0, 0.0, 0.0));
	door->SetSize(PERVec3(1.0, 3.0, 0.1));
	dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi())->SetExcuteFunc([](ResponeseToSignalAiComponent* component) {
		// 문을 통과 가능하게 변경
		component->GetOwner()->SetSize(PERVec3(1.0, 1.0, 0.0));
		component->GetOwner()->SetPosition(PERVec3(9.0, -1.0, 0.1));
		});
	dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi())->SetRevokeFunc([](ResponeseToSignalAiComponent* component) {
		// 문을 통과 불가능하게 변경
		component->GetOwner()->SetSize(PERVec3(1.0, 3.0, 0.0));
		component->GetOwner()->SetPosition(PERVec3(9.0, 0.0, 0.0));
		});
	AddObject(door);

	// 버튼
	PERObject* button;
	button = m_objectStorage->PopObject(PERObjectType::BUTTON);
	button->SetPosition(PERVec3(7.0, 0.0, 0.0));
	PERComponent::AiData aiData;
	aiData.isSwitch = true;
	button->GetAi().SetData(aiData);
	PERComponent::GraphicsData onData;
	PERComponent::GraphicsData offData;
	onData.color = PERColor(200, 50, 50);
	offData.color = PERColor(100, 100, 100);
	button->GetGraphics().SetData(offData);
	dynamic_cast<MakingSignalAiComponent*>(&button->GetAi())->SetOnOffGraphicsData(onData, offData);
	dynamic_cast<MakingSignalAiComponent*>(&button->GetAi())->SetResponseAi(dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi()));
	AddObject(button);

	// 압력
	PERObject* pressure;
	pressure = m_objectStorage->PopObject(PERObjectType::PRESSURE);
	pressure->SetPosition(PERVec3(7.0, 1.5, 0.0));
	onData.color = PERColor(150, 150, 200);
	offData.color = PERColor(200, 200, 200);
	pressure->GetGraphics().SetData(offData);
	dynamic_cast<MakingSignalAiComponent*>(&pressure->GetAi())->SetOnOffGraphicsData(onData, offData);
	dynamic_cast<MakingSignalAiComponent*>(&pressure->GetAi())->SetResponseAi(dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi()));
	AddObject(pressure);

	// 트리거
	PERObject* trigger;
	trigger = m_objectStorage->PopObject(PERObjectType::TRIGGER);
	trigger->SetPosition(PERVec3(12.0, 0, 0.05));
	trigger->SetSize(PERVec3(1.0, 5.0, 0.0));
	ResponeseToSignalAiComponent* triggerAI = dynamic_cast<ResponeseToSignalAiComponent*>(trigger->GetAi().GetNextComponent());
	triggerAI->SetExcuteFunc([](ResponeseToSignalAiComponent* component) {
		EventDispatcher::Send(PEREvent::RUN_TEST2_WORLD, PERVec3());
		});
	triggerAI->SetRevokeFunc([](ResponeseToSignalAiComponent* component) {
		
		});
	dynamic_cast<MakingSignalAiComponent*>(&trigger->GetAi())->SetResponseAi(triggerAI);
	AddObject(trigger);
}
