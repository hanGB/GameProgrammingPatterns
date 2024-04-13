#include "stdafx.h"
#include "ui_element.h"

UiElement::UiElement()
{

}

UiElement::~UiElement()
{
	
}

bool UiElement::GetIsLiving() const
{
	return m_isLiving;
}

void UiElement::SetPosition(PERVec2 position)
{
	m_position = position;
}

void UiElement::SetSize(PERVec2 size)
{
	m_size = size;
}

void UiElement::SetType(PERUiElementType type)
{
	m_type = type;
}

void UiElement::SetIsLiving(bool living)
{
	m_isLiving = living;
}

PERUiElementType UiElement::GetType() const
{
	return m_type;
}
