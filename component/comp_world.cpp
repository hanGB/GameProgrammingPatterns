#include "stdafx.h"
#include "comp_world.h"

CompWorld::CompWorld()
{
	m_groundPosX = 0.0f; m_groundPosY = -1.0;
	m_groundWidth = 5.0; m_groundHeight = 0.5f;
	m_groundColor.red = 127; m_groundColor.green = 63; m_groundColor.blue = 0;
}

CompWorld::~CompWorld()
{
}

void CompWorld::Update(double elapsedTimeInSec)
{
}

void CompWorld::RenderWorld(CompRenderer& renderer)
{
	renderer.RenderRectangle(m_groundPosX, m_groundPosY, m_groundWidth, m_groundHeight, m_groundColor);
}

bool CompWorld::CheckCollision(double* posX, double* posY, double sizeX, double sizeY)
{
	// 땅과 충돌처리
	if (IsCollidedWithAABB(*posX, *posY, sizeX, sizeY, 
		m_groundPosX, m_groundPosY, m_groundWidth, m_groundHeight)) {
		
		MoveOneObjectToAdjustPosition(posX, posY, sizeX, sizeY,
			m_groundPosX, m_groundPosY, m_groundWidth, m_groundHeight);
		
		return true;
	}
	
	return false;
}

bool CompWorld::IsCollidedWithAABB(
	double aPosX, double aPosY, double aSizeX, double aSizeY, 
	double bPosX, double bPosY, double bSizeX, double bSizeY
)
{
	double aHalfSizeX = aSizeX / 2.0, aHalfSizeY = aSizeY / 2.0;
	double bHalfSizeX = bSizeX / 2.0, bHalfSizeY = bSizeY / 2.0;

	if (aPosX + aHalfSizeX < bPosX - bHalfSizeX) return false;
	if (aPosX - aHalfSizeX > bPosX + bHalfSizeX) return false;
	if (aPosY + aHalfSizeY < bPosY - bHalfSizeY) return false;
	if (aPosY - aHalfSizeY > bPosY + bHalfSizeY) return false;
	return true;
}

void CompWorld::MoveOneObjectToAdjustPosition(
	double* movableObjectPosX, double* movableObjectPosY, double movableObjectSizeX, double movableObjectSizeY,
	double fixedObjectPosX, double fixedObjectPosY, double fixedObjectSizeX, double fixedObjectSizeY)
{
	double mHalfSizeX = movableObjectSizeX / 2.0, mHalfSizeY = movableObjectSizeY / 2.0;
	double fHalfSizeX = fixedObjectSizeX / 2.0, fHalfSizeY = fixedObjectSizeY / 2.0;

	if (fixedObjectPosY + fHalfSizeY > *movableObjectPosY - mHalfSizeY) {
		*movableObjectPosY = *movableObjectPosY + (fixedObjectPosY + fHalfSizeY - (*movableObjectPosY - mHalfSizeY));
	}
}

