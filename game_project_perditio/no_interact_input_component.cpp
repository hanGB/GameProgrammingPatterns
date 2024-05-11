#include "stdafx.h"
#include "no_interact_input_component.h"

void NoInteractInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	InputComponent::Update(world, controller, audio, dTime);
}

void NoInteractInputComponent::SetData(PERComponent::InputData data)
{
	InputComponent::SetData(data);
}

void NoInteractInputComponent::Initialize()
{
	InputComponent::Initialize();
}
