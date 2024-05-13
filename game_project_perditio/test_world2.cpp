#include "stdafx.h"
#include "test_world2.h"
#include "game_mode.h"
#include "per_object.h"
#include "object_storage.h"
#include "spawner_ai_component.h"
#include "test_game_mode.h"
#include "making_signal_ai_component.h"
#include "response_to_signal_ai_component.h"
#include "event_dispatcher.h"

TestWorld2::TestWorld2(ObjectStorage* objectStorage, PERDatabase* database)
{
	InitSettingForWorld(objectStorage, database, new TestGameMode());
}

TestWorld2::~TestWorld2()
{
}

void TestWorld2::Enter()
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(-8.0, 0.0, 0.1));
	m_gameMode->GetPlayer().SetCurrentPositionToSpawnPosition();

	PERWorld::Enter();
}

void TestWorld2::Exit()
{
	PERWorld::Exit();
}

void TestWorld2::Pause()
{
	m_playerPosBeforePause = m_gameMode->GetPlayer().GetPosition();
	PERWorld::Pause();
}

void TestWorld2::Resume()
{
	m_gameMode->GetPlayer().SetPosition(m_playerPosBeforePause);
	PERWorld::Resume();
}

void TestWorld2::AddFixedAndPhysicalObjects()
{
	// 플랫폼
	PERObject* platform;
	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	platform->SetPosition(PERVec3(0.0, 0.0, -1.0));
	platform->SetSize(PERVec3(10.0, 10.0, 1.0));
	SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
	AddObject(platform);

	platform = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	platform->SetPosition(PERVec3(-7.5, 0.0, -1.0));
	platform->SetSize(PERVec3(5.0, 5.0, 1.0));
	SetObjectShapeAndColor(platform, PERShapeType::ROUND_RECTANGLE, PERColor(250, 230, 210));
	AddObject(platform);
	
	// 벽
	PERObject* wall;
	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(5.0, 0.0, 0.0));
	wall->SetSize(PERVec3(0.5, 10.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-5.0, 3.0, 0.1));
	wall->SetSize(PERVec3(0.5, 4.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(-5.0, -3.0, 0.1));
	wall->SetSize(PERVec3(0.5, 4.0, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, 5.0, 0.2));
	wall->SetSize(PERVec3(10.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);

	wall = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	wall->SetPosition(PERVec3(0.0, -5.0, 0.3));
	wall->SetSize(PERVec3(10.0, 0.5, 1.0));
	SetObjectShapeAndColor(wall, PERShapeType::RECTANGLE, PERColor(150, 125, 100));
	AddObject(wall);
}

void TestWorld2::AddOtherObjects()
{
	// 몬스터 생성기
	PERObject* monsterSpanwer;
	monsterSpanwer = m_objectStorage->PopObject(PERObjectType::SPAWNER);
	monsterSpanwer->SetPosition(PERVec3(0.0, 0.0, 0.1));
	dynamic_cast<SpawnerAiComponent*>( &monsterSpanwer->GetAi() )->SetSpawner(
	"MONSTER_NIKKEL", PERObjectType::MONSTER, PERSpawnType::LIVE, 1
	);
	AddObject(monsterSpanwer);

	// 트리거
	PERObject* trigger;
	trigger = m_objectStorage->PopObject(PERObjectType::TRIGGER);
	trigger->SetPosition(PERVec3(-10.0, 0, 0.05));
	trigger->SetSize(PERVec3(1.0, 5.0, 0.0));
	ResponeseToSignalAiComponent* triggerAI = dynamic_cast< ResponeseToSignalAiComponent* >( trigger->GetAi().GetNextComponent() );
	triggerAI->SetExcuteFunc([ ] (ResponeseToSignalAiComponent* component) {
		EventDispatcher::Send(PEREvent::QUIT_WORLD, PERVec3());
		});
	triggerAI->SetRevokeFunc([ ] (ResponeseToSignalAiComponent* component) {

		});
	dynamic_cast< MakingSignalAiComponent* >( &trigger->GetAi() )->SetResponseAi(triggerAI);
	AddObject(trigger);

	// 문
	PERObject* door;
	door = m_objectStorage->PopObject(PERObjectType::DOOR);
	door->SetPosition(PERVec3(-5.0, 0.0, 0.0));
	door->SetSize(PERVec3(0.75, 2.0, 0.1));
	dynamic_cast< ResponeseToSignalAiComponent* >( &door->GetAi() )->SetExcuteFunc([ ] (ResponeseToSignalAiComponent* component) {
		// 문을 통과 가능하게 변경
		component->GetOwner()->SetSize(PERVec3(0.75, 0.5, 0.0));
		component->GetOwner()->SetPosition(PERVec3(-5.0, -0.85, 0.1));
		});
	dynamic_cast< ResponeseToSignalAiComponent* >( &door->GetAi() )->SetRevokeFunc([ ] (ResponeseToSignalAiComponent* component) {
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
	dynamic_cast< MakingSignalAiComponent* >( &button->GetAi() )->SetOnOffGraphicsData(onData, offData);
	dynamic_cast< MakingSignalAiComponent* >( &button->GetAi() )->SetResponseAi(dynamic_cast< ResponeseToSignalAiComponent* >( &door->GetAi() ));
	AddObject(button);
}
