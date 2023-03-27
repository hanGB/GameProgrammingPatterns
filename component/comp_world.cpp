#include "stdafx.h"
#include "comp_world.h"

CompWorld::CompWorld()
{
	m_groundPosition = { 0.0, -1.0 };
	m_groundSize = { 5.0, 0.5 };
	m_groundColor = { 127, 63, 0 };
}

CompWorld::~CompWorld()
{
}

void CompWorld::Update(double elapsedTimeInSec)
{
}

void CompWorld::RenderWorld(CompRenderer& renderer)
{
	renderer.RenderRectangle(m_groundPosition, m_groundSize, m_groundColor);
}

bool CompWorld::CheckCollision(CompVector3<double>* pos, CompVector2<double> volme)
{
	// 땅과 충돌처리
	CompVector2<double> groundVolume;
	groundVolume.x = m_groundSize.x;
	groundVolume.y = m_groundSize.y;

	if (IsCollidedWithAABB(*pos, volme,
		m_groundPosition, groundVolume)) {

		MoveOneObjectToAdjustPosition(pos, volme,
			m_groundPosition, groundVolume);

		return true;
	}

	return false;
}

bool CompWorld::IsCollidedWithAABB(
	CompVector3<double> aPos, CompVector2<double> aVolme,
	CompVector3<double> bPos, CompVector2<double> bVolme)
{

	double aHalfSizeX = aVolme.x / 2.0, aHalfSizeY = aVolme.y / 2.0;
	double bHalfSizeX = bVolme.x / 2.0, bHalfSizeY = bVolme.y / 2.0;

	if (aPos.x + aHalfSizeX < bPos.x - bHalfSizeX) return false;
	if (aPos.x - aHalfSizeX > bPos.x + bHalfSizeX) return false;
	if (aPos.y + aHalfSizeY < bPos.y - bHalfSizeY) return false;
	if (aPos.y - aHalfSizeY > bPos.y + bHalfSizeY) return false;
	return true;
}

void CompWorld::MoveOneObjectToAdjustPosition(
	CompVector3<double>* movablePos, CompVector2<double> movableVolme, 
	CompVector3<double> fixedPos, CompVector2<double> fixedVolme)
{
	double mHalfSizeX = movableVolme.x / 2.0, mHalfSizeY = movableVolme.y / 2.0;
	double fHalfSizeX = fixedVolme.x / 2.0, fHalfSizeY = fixedVolme.y / 2.0;

	if (fixedPos.y + fHalfSizeY > movablePos->y - mHalfSizeY) {
		movablePos->y = movablePos->y + (fixedPos.y + fHalfSizeY - (movablePos->y - mHalfSizeY));
	}
}


