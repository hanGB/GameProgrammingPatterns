#pragma once
#include "object.h"

class Food : public Object {
public:
	Food(int x, int y) { SetPos(x, y); }
	~Food() {}

	virtual void Render() {
		int x, y;
		GetPos(&x, &y);
		MoveCurser(x, y);
		std::cout << "F";
	}
};