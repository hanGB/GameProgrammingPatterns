#include "stdafx.h"
#include "button_input_component.h"
#include "black_board.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void ButtonInputComponent::Update(PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(KeySetting::Use.value)) {
		PERVec3 playerPos = BlackBoard::GetPlayerPos();
		PERVec3 pos = GetOwner()->GetPosition();

		if (c_DEFAULT_INPUT_RANGE_SQURE > DistanceSquareAandBIgnoringZValue(playerPos, pos))
		{
			dynamic_cast<MakingSignalAiComponent*>(&GetOwner()->GetAi())->SetIsGetInput(true);
		}
	}
}

void ButtonInputComponent::SetData(PERComponent::InputData data)
{
}

void ButtonInputComponent::Initialize(PERComponent::InputData data)
{
	SetData(data);
}
