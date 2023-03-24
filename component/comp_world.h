#pragma once
#include "stdafx.h"
#include "comp_renderer.h"

class CompWorld {
public:
	CompWorld();
	~CompWorld();

	void Update(double elapsedTimeInSec);
	void RenderWorld(CompRenderer& renderer);
	bool CheckCollision(double* posX, double* posY, double sizeX, double sizeY);

private:
	bool IsCollidedWithAABB(double aPosX, double aPosY, double aSizeX, double aSizeY,
		double bPosX, double bPosY, double bSizeX, double bSizeY);

	void MoveOneObjectToAdjustPosition(
		double* movableObjectPosX, double* movableObjectPosY, double movableObjectSizeX, double movableObjectSizeY,
		double fixedObjectPosX, double fixedObjectPosY, double fixedObjectSizeX, double fixedObjectSizeY);

	double m_groundPosX, m_groundPosY;
	double m_groundWidth, m_groundHeight;
	CompRGBColor m_groundColor;
};