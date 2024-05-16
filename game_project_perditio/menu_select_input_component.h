#pragma once
#include "input_component.h"

class MenuSelectInputComponent : public InputComponent {
public:
	~MenuSelectInputComponent();

	virtual void Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime);
	virtual void SetData(PERComponent::InputData data);
	virtual void Initialize();

	void AddMenu(std::function<void(PERWorld&, PERController&, PERAudio&, double)> func);

private:
	std::vector<std::function<void(PERWorld&, PERController&, PERAudio&, double)>> m_menus;
	int m_selectedMenu = 0;
	bool m_isSelected = false;
};
