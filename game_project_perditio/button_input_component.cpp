#include "stdafx.h"
#include "button_input_component.h"
#include "per_controller.h"
#include "black_board.h"
#include "per_object.h"
#include "making_signal_ai_component.h"

void ButtonInputComponent::Update(PERObject& object, PERWorld& world, PERController& controller, PERAudio& audio, double dTime)
{
	if (controller.IsKeyboardPressedRightNow(m_inputKeyValue)) {
		PERVec3 playerPos = BlackBoard::GetPlayerPos();
		PERVec3 pos = object.GetPosition();

		if (1.0 > DistanceSquareAandBIgnoringZValue(playerPos, pos))
		{
			PERLog::Logger().Info("버튼이 눌림");
			dynamic_cast<MakingSignalAiComponent*>(&object.GetAi())->SetIsGetInput(true);
		}
	}
}

void ButtonInputComponent::SetData(PERComponent::InputData data)
{
}
