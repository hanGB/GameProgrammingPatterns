#include "stdafx.h"
#include "key_input_helper.h"
#include "per_controller.h"
#include "per_renderer.h"

KeyInputHelper::KeyInputHelper()
{
}

KeyInputHelper::~KeyInputHelper()
{
}

void KeyInputHelper::Init(PERVec2 pos, PERVec2 size, PERColor defaultColor, PERColor pressedColor, 
	PERKeyboardData key, double fontSize, PERColor defaultTextColor, PERColor pressedTextColor)
{
	UiElement::Init(pos, size, defaultColor);
	m_pressedColor = pressedColor;
	m_key = key;
	m_fontSize = fontSize;
	m_defaultTextColor = defaultTextColor;
	m_pressedTextColor = pressedTextColor;
	m_isPressed = false;
}

void KeyInputHelper::MatchWithData(PERVec2 pos)
{
	// 대부분의 경우 키 입력 도우미는 처음 정한 위치를 변경할 일 없음
	SetPosition(pos);
}


void KeyInputHelper::Update(PERController& controller, PERAudio& audio, double dTime)
{
	if (!GetIsInUse()) return;

	if (controller.IsKeyboardPressed(m_key.value, true)) m_isPressed = true;
	else m_isPressed = false;
}

void KeyInputHelper::RenderOnScreen(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();

	PERColor background = GetBackgroundColor();
	PERColor textColor = m_defaultTextColor;
	if (m_isPressed)
	{
		background = m_pressedColor;
		textColor = m_pressedTextColor;
	}

	// 배경
	renderer.RenderShapeInScreenCoordinate(
		PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR, pos, size, background, false);
	// 키
	renderer.RenderFontInScreenCoordinate(m_key.show.c_str(), (int)m_key.show.size(), m_fontSize, PERVec2(pos.x - size.x * 0.2, pos.y), textColor);
}

void KeyInputHelper::RenderInWorld(PERRenderer& renderer, PERDatabase& database)
{
	if (!GetIsInUse()) return;

	PERVec2 pos = GetPosition();
	PERVec2 size = GetSize();

	PERColor background = GetBackgroundColor();
	PERColor textColor = m_defaultTextColor;
	if (m_isPressed)
	{
		background = m_pressedColor;
		textColor = m_pressedTextColor;
	}

	// 배경
	renderer.RenderShapeInWorldCoordinate(
		PERShapeType::RECTANGLE_WITH_LEFT_TOP_ANCHOR, PERVec3(pos.x - size.x * 0.5, pos.y, 0.0), PERVec3(size.x, size.y, 0.0), background, false);
	// 키
	renderer.RenderFontInWorldCoordinate(m_key.show.c_str(), (int)m_key.show.size(), m_fontSize,
		PERVec2(pos.x - size.x * 0.2, pos.y), textColor);

}
