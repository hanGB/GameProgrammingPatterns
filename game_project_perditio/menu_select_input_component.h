#pragma once
#include "input_component.h"

class MenuSelectInputComponent : public InputComponent {
public:
	~MenuSelectInputComponent();

	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize();

	void AddMenu(std::function<void(PERWorld&, PERAudio&, double)> func);
	void SetESCMenu(std::function<void(PERWorld&, PERAudio&, double)> func);

private:
	void MoveInMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	void UseSelectedMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	void UseESCMenu(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);

	std::vector<std::function<void(PERWorld&, PERAudio&, double)>> m_menus;
	int m_selectedMenu = 0;

	std::function<void(PERWorld&, PERAudio&, double)> m_escMenu;
};
