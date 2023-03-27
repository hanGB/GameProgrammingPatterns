#pragma once
#include "stdafx.h"
#include "comp_renderer.h"

class CompWorld {
public:
	CompWorld();
	~CompWorld();

	void Update(double elapsedTimeInSec);
	void RenderWorld(CompRenderer& renderer);
	bool CheckCollision(CompVector3<double>* pos, CompVector2<double> volme);

private:
	bool IsCollidedWithAABB(CompVector3<double> aPos, CompVector2<double> aVolme,
		CompVector3<double> bPos, CompVector2<double> bVolme);

	void MoveOneObjectToAdjustPosition(
		CompVector3<double>* movablePos, CompVector2<double> movableVolme,
		CompVector3<double> fixedPos, CompVector2<double> fixedVolme);

	CompVector3<double> m_groundPosition;
	CompVector2<double> m_groundSize;
	CompShapeType m_groundShapeType;
	CompColor m_groundColor;
};