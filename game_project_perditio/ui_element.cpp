#include "stdafx.h"
#include "ui_element.h"

UiElement::UiElement()
{

}

UiElement::~UiElement()
{
	
}

void UiElement::SetPosition(PERVec2 position)
{
	m_position = position;
}

void UiElement::SetSize(PERVec2 size)
{
	m_size = size;
}
