#include "stdafx.h"
#include "button_input_component.h"
#include "black_board.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void ButtonInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(KeySetting::Use.value)) {
		PERVec3 playerPos = BlackBoard::GetPlayerPos();
		PERVec3 pos = object.GetPosition();

		if (c_DEFAULT_INPUT_RANGE_SQURE > DistanceSquareAandBIgnoringZValue(playerPos, pos))
		{
			dynamic_cast<MakingSignalAiComponent*>(&object.GetAi())->SetIsGetInput(true);
		}
	}
}

void ButtonInputComponent::SetData(PERComponent::InputData data)
{
}
