#include "credits_world.h"
#include "object_storage.h"
#include "event_dispatcher.h"
#include "text_graphics_component.h"
#include "response_to_signal_ai_component.h"
#include "making_signal_ai_component.h"
#include "hidden_debug_graphics_component.h"
#include "credits_game_mode.h"
#include "null_hud.h"
#include "test_game_state.h"

CreditsWorld::CreditsWorld(ObjectStorage* objectStorage, PERDatabase* database)
{
	CreditsGameMode* gameMode = new CreditsGameMode();
	gameMode->SetHud(new NullHud());
	gameMode->SetGameState(new TestGameState());

	InitSettingForWorld(objectStorage, database, gameMode);
}

CreditsWorld::~CreditsWorld()
{

}

void CreditsWorld::Enter(PERRenderer& renderer, PERAudio& audio)
{
	m_gameMode->GetPlayer().SetPosition(PERVec3(0.0, 20.0, 0.1));

	PERWorld::Enter(renderer, audio);
}

void CreditsWorld::Exit(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Exit(renderer, audio);
}

void CreditsWorld::Pause(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Pause(renderer, audio);
}

void CreditsWorld::Resume(PERRenderer& renderer, PERAudio& audio)
{
	PERWorld::Resume(renderer, audio);
}

void CreditsWorld::InitWorldObject()
{
	MakeBackground();
	MakeCredits();
}

void CreditsWorld::MakeBackground()
{
	// 배경
	PERObject* background;
	background = m_objectStorage->PopObject(PERObjectType::FIXED_BLOCK);
	SetObjectVisual(background, "CREDITS_BACKGOUND");
	background->SetPosition(PERVec3(0.0, 0.0, -1.0));
	background->SetSize(PERVec3(20.0, 60.0, 1.0));
	background->SetMass(PER_FIXED_OBJECT_MASS);
	AddObject(background);
}

void CreditsWorld::MakeCredits()
{
	std::wstring credit[5] = { L"Producter: Kim", L"Director: Lee", L"Programmer: Park", L"Studio IM", L"Thanks you for playing!" };

	// 크리딧
	for (int i = 0; i < 5; ++i) {
		PERObject* text;
		text = m_objectStorage->PopObject(PERObjectType::TEXT);
		text->SetPosition(PERVec3(-5.0, 10.0 + i * -5.0, 1.0));
		text->SetSize(PERVec3(1.0, 0.0, 0.0));
		dynamic_cast<TextGraphicsComponent*>(&text->GetGraphics())->SetText(credit[i].c_str(), PERColor(255, 255, 255));
		AddObject(text);
	}

	// 타이틀로 보내 주는 트리거
	PERObject* trigger;
	trigger = m_objectStorage->PopObject(PERObjectType::TRIGGER);
	trigger->SetPosition(PERVec3(0.0, -20.0, 0.05));
	trigger->SetSize(PERVec3(30.0, 1.0, 0.0));
	ResponeseToSignalAiComponent* triggerAI = dynamic_cast<ResponeseToSignalAiComponent*>(trigger->GetAi().GetNextComponent());
	triggerAI->SetExcuteFunc([](ResponeseToSignalAiComponent* component) {
		EventDispatcher::Send(PEREvent::BACK_TO_TITLE, PERVec3());
		});
	triggerAI->SetRevokeFunc([](ResponeseToSignalAiComponent* component) {

		});
	dynamic_cast<MakingSignalAiComponent*>(&trigger->GetAi())->SetResponseAi(triggerAI);
	dynamic_cast<HiddenDebugGraphicsComponent*>(&trigger->GetGraphics())->SetDebug(false);
	AddObject(trigger);
}

