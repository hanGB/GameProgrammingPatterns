#pragma once
#include "object.h"
#include "snake.h"

class Tail : public Snake {
public:
	Tail(Snake &last)
	{
		int x, y;
		last.GetPrevPos(&x, &y);
		SetPos(x, y);
		SetType(ObjectType::Tail);
	}
	~Tail() { }

	void Move(Snake &last)
	{
		int x, y;
		SaveCurrentPos();
		last.GetPrevPos(&x, &y);
		SetPos(x, y);
	}
	
	virtual void Render() 
	{
		int x, y;
		GetPos(&x, &y);
		MoveCurser(x, y);
		std::cout << "T";
	}

private:

};