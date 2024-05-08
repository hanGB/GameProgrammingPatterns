#include "stdafx.h"
#include "ui_element.h"

UiElement::UiElement()
{

}

UiElement::~UiElement()
{
	
}

UiElement* UiElement::GetNext() const
{
	return m_next;
}

void UiElement::SetNext(UiElement* next)
{
	m_next = next;
}

bool UiElement::GetIsInUse() const
{
	return m_isInUse;
}

void UiElement::SetIsInUse(bool inUse)
{
	m_isInUse = inUse;
}

void UiElement::Init(PERVec2 pos, PERVec2 size, PERColor backgroundColor, bool border, int borderWidth, PERColor borderColor)
{
	 m_position = pos;
	 m_size = size;
	 m_backgroundColor = backgroundColor;

	// ׵θ
	 m_border = border;
	 m_borderWidth = borderWidth;
	 m_borderColor = borderColor;

	 m_lifeTime = PER_MAXIMUM_LIFE_TIME;
	 m_isInUse = true;
}

bool UiElement::IsLifeTimeEnd(double time)
{
	m_lifeTime -= time;

	return 0.0 > m_lifeTime;
}

PERVec2 UiElement::GetPosition() const
{
	return m_position;
}

PERVec2 UiElement::GetSize() const
{
	return m_size;
}

PERColor UiElement::GetBackgroundColor() const
{
	return m_backgroundColor;
}

bool UiElement::GetBorder() const
{
	return m_border;
}

int UiElement::GetBorderWidth() const
{
	return m_borderWidth;
}

PERColor UiElement::GetBorderColor() const
{
	return  m_borderColor;
}

double UiElement::GetLifeTime() const
{
	return  m_lifeTime;
}

void UiElement::SetPosition(PERVec2 position)
{
	 m_position = position;
}

void UiElement::SetSize(PERVec2 size)
{
	 m_size = size;
}

void UiElement::SetBackgroundColor(PERColor color)
{
	 m_backgroundColor = color;
}

void UiElement::SetBorder(bool border, int width, PERColor color)
{
	 m_border = border;
	 m_borderWidth = width;
	 m_borderColor = color;
}

void UiElement::SetLifeTime(double time)
{
	 m_lifeTime = time;
}
