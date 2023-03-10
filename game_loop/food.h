#pragma once
#include "object.h"

class Food : public Object {
public:
	Food(int x, int y) { SetPos(x, y); SetType(ObjectType::Food); }
	~Food() {}

	virtual void Render(Renderer& renderer, double differenceRate) {
		int x, y;
		GetPos(&x, &y);
		renderer.PrintOnBuffer(x, y, "F");
	}
};