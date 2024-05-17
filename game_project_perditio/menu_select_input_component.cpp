#include "stdafx.h"
#include "menu_select_input_component.h"
#include "per_controller.h"
#include "event_dispatcher.h"

MenuSelectInputComponent::~MenuSelectInputComponent()
{
	m_menus.clear();
	m_menus.shrink_to_fit();
}

void MenuSelectInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{

	MoveInMenu(world, controller, audio, dTime);
	UseSelectedMenu(world, controller, audio, dTime);
	UseESCMenu(world, controller, audio, dTime);

	InputComponent::Update(world, controller, audio, dTime);
}

void MenuSelectInputComponent::SetData(PERComponent::InputData data)
{
	InputComponent::SetData(data);
}

void MenuSelectInputComponent::Initialize()
{
	m_selectedMenu = 0;
	InputComponent::Initialize();
}

void MenuSelectInputComponent::AddMenu(std::function<void(PERWorld&, PERAudio&, double)> func)
{
	m_menus.push_back(func);
}

void MenuSelectInputComponent::SetESCMenu(std::function<void(PERWorld&, PERAudio&, double)> func)
{
	m_escMenu = func;
}

void MenuSelectInputComponent::MoveInMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	int tempIndex = m_selectedMenu;

	if (controller.IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue::UP)) {
		m_selectedMenu--;
		m_selectedMenu = std::clamp(m_selectedMenu, 0, (int)m_menus.size() - 1);
	}
	if (controller.IsKeyboardPressedRightNowOrMoreThanTime(PERKeyboardValue::DOWN)) {
		m_selectedMenu++;
		m_selectedMenu = std::clamp(m_selectedMenu, 0, (int)m_menus.size() - 1);
	}

	if (tempIndex != m_selectedMenu)
	{
		EventDispatcher::Send(PEREvent::SET_SELECTED_MENU, PERVec3((double)m_selectedMenu, 0.0, 0.0));
	}
}

void MenuSelectInputComponent::UseSelectedMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(PERKeyboardValue::SPACE)) {
		m_menus[m_selectedMenu](world, audio, dTime);
	}
}

void MenuSelectInputComponent::UseESCMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(PERKeyboardValue::ESC)) {
		m_escMenu(world, audio, dTime);
	}

}
