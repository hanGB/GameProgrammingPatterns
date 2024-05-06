#include "stdafx.h"
#include "no_interact_input_component.h"

void NoInteractInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
}

void NoInteractInputComponent::SetData(PERComponent::InputData data)
{
}

void NoInteractInputComponent::Initialize(PERComponent::InputData data)
{
	SetData(data);
}
