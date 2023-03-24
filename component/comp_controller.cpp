#include "stdafx.h"
#include "comp_controller.h"

CompController* CompController::GetInstance()
{
	static CompController instance;
	return &instance;
}

CompInputChunk CompController::GetInputs() const
{
	return m_inputs;
}

void CompController::SetInputs(CompInputChunk& inputs)
{
	m_inputs = inputs;
}

int CompController::GetXAxisDirection()
{
	if (m_inputs.Input_A && !m_inputs.Input_D) {
		return -1;
	}
	else if (!m_inputs.Input_A && m_inputs.Input_D) {
		return 1;
	}
	else {
		return 0;
	}
}

int CompController::GetYAxisDirection()
{
	if (m_inputs.Input_S && !m_inputs.Input_W) {
		return -1;
	}
	else if (!m_inputs.Input_S && m_inputs.Input_W) {
		return 1;
	}
	else {
		return 0;
	}
}

CompController::CompController()
{
}
CompController::~CompController()
{
}
