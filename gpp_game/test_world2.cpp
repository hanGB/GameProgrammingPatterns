#include "stdafx.h"
#include "test_world2.h"
#include "test_game_mode.h"
#include "test_game_state.h"
#include "test_field_hud.h"
#include "per_object.h"
#include "object_storage.h"
#include "spawner_ai_component.h"
#include "test_game_mode.h"
#include "making_signal_ai_component.h"
#include "response_to_signal_ai_component.h"
#include "event_dispatcher.h"
#include "player_state.h"
#include "black_board.h"

TestWorld2::TestWorld2(ObjectStorage* objectStorage, PERDatabase* database)
{
	TestGameMode* gameMode = new TestGameMode();
	gameMode->SetGameState(new TestGameState());
	gameMode->SetHud(new TestFieldHud());

	InitSettingForWorld(objectStorage, database, gameMode);
}

TestWorld2::~TestWorld2()
{
}

void TestWorld2::Enter(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(-9.0, 0.0, 0.1));
	m_gameMode->GetPlayerState().SetCurrentWorldType(PERWorldType::TEST_WORLD2);
	m_gameMode->GetPlayer().SetCurrentPositionToSpawnPosition();

	PERWorld::Enter(renderer, audio);
}

void TestWorld2::Exit(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Exit(renderer, audio);
}

void TestWorld2::Pause(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->GetPlayerState().SetCurrentWorldType(PERWorldType::TEST_WORLD2);
	m_playerPosBeforePause = m_gameMode->GetPlayer().GetPosition();
	PERWorld::Pause(renderer, audio);
}

void TestWorld2::Resume(PERRenderer& renderer, PERAudio& audio)
{
	// 네비게이션 데이터 설정
	BlackBoard::GetNavigationData().ReadDataFromFile("./map/test.nv");
	m_gameMode->GetPlayer().SetPosition(m_playerPosBeforePause);
	PERWorld::Resume(renderer, audio);
}

void TestWorld2::InitWorldObject()
{
	// 네비게이션 데이터에 영향을 주는 오브젝트 먼저 추가
	ReadFixedObjectsDataFromFile("./map/test2_fixed.map");

	// 네비게이션 데이터 설정
	BlackBoard::GetNavigationData().ReadDataFromFile("./map/test2.nv");

	// 네비게이션 데이터와 상관없는 나머지 오브젝트 추가
	AddOtherObjects();
}

void TestWorld2::AddOtherObjects()
{
	// 몬스터 생성기
	PERObject* monsterSpanwer;
	monsterSpanwer = m_objectStorage->PopObject(PERObjectType::SPAWNER);
	monsterSpanwer->SetPosition(PERVec3(0.0, 0.0, 0.2));
	dynamic_cast<SpawnerAiComponent*>(&monsterSpanwer->GetAi())->SetSpawner(
		"MONSTER_NIKKEL", PERObjectType::MONSTER, PERSpawnType::LIVE, 1
	);
	AddObject(monsterSpanwer);

	PERObject* block;
	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-7.5, 1.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	block = m_objectStorage->PopObject(PERObjectType::MOVABLE_BLOCK);
	block->SetPosition(PERVec3(-7.5, -1.0, 0.1));
	SetObjectShapeAndColor(block, PERShapeType::RECTANGLE, PERColor(150, 200, 150), true, 1, PERColor(0, 250, 0));
	AddObject(block);

	// 트리거
	PERObject* trigger;
	trigger = m_objectStorage->PopObject(PERObjectType::TRIGGER);
	trigger->SetPosition(PERVec3(-10.0, 0, 0.05));
	trigger->SetSize(PERVec3(1.0, 5.0, 0.0));
	ResponeseToSignalAiComponent* triggerAI = dynamic_cast<ResponeseToSignalAiComponent*>(trigger->GetAi().GetNextComponent());
	triggerAI->SetExcuteFunc([](ResponeseToSignalAiComponent* component) {
		EventDispatcher::Send(PEREvent::RUN_TEST_WORLD, PERVec3());
		});
	triggerAI->SetRevokeFunc([](ResponeseToSignalAiComponent* component) {

		});
	dynamic_cast<MakingSignalAiComponent*>(&trigger->GetAi())->SetResponseAi(triggerAI);
	AddObject(trigger);

	// 문
	PERObject* door;
	door = m_objectStorage->PopObject(PERObjectType::DOOR);
	door->SetPosition(PERVec3(-5.0, 0.0, 0.0));
	door->SetSize(PERVec3(0.75, 2.0, 0.1));
	dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi())->SetExcuteFunc([](ResponeseToSignalAiComponent* component) {
		// 문을 통과 가능하게 변경
		component->GetOwner()->SetSize(PERVec3(0.75, 0.5, 0.0));
		component->GetOwner()->SetPosition(PERVec3(-5.0, -0.85, 0.1));
		});
	dynamic_cast<ResponeseToSignalAiComponent*>(&door->GetAi())->SetRevokeFunc([](ResponeseToSignalAiComponent* component) {
		// 문을 통과 불가능하게 변경
		component->GetOwner()->SetSize(PERVec3(0.75, 2.0, 0.0));
		component->GetOwner()->SetPosition(PERVec3(-5.0, 0.0, 0.0));
		});
	AddObject(door);

	// 버튼
	PERObject* button;
	button = m_objectStorage->PopObject(PERObjectType::BUTTON);
	button->SetPosition(PERVec3(-6.0, -1.75, 0.0));
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
}
