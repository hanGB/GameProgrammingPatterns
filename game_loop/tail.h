#pragma once
#include "object.h"
#include "snake.h"

class Tail : public Snake {
public:
	Tail(Snake &last)
	{
		int x, y;
		int prevX, prevY;
		last.GetPos(&x, &y);
		last.GetPrevPos(&prevX, &prevY);
		SetPos(prevX, prevY);
		SetPrevPos(prevX - (x - prevX), prevY - (y - prevY));
		SetType(ObjectType::Tail);
	}
	~Tail() { }

	void Move(Snake& last)
	{
		int x, y;
		SavePrevAndCurrentPos();
		last.GetPrevPos(&x, &y);
		SetPos(x, y);
	}
	void Back()
	{
		int x, y;
		GetPrevPos(&x, &y); 
		SetPos(x, y);
		SavePrevPrevPosInPrevPos();
	}
	
	virtual void Render(Renderer& renderer, double differenceRate)
	{
		int x, y;
		GetPos(&x, &y);
		renderer.PrintOnBuffer(x, y, "T");
	}

private:

};