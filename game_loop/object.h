#pragma once
#include "stdafx.h"
#include "renderer.h"

enum class ObjectType {
	Player,
	Tail,
	Food
};

class Object {
public:
	virtual void ProcessInputs(KeyInputs& inputs, double elapsedTimeInSec) {}
	virtual void Update(double elapsedTimeInSec) {}
	virtual void Render(Renderer& renderer) {}

	virtual void CollidideWithType(ObjectType type) {}

	void SetPos(int x, int y) {
		m_posX = x;
		m_posY = y;
	}
	void GetPos(int* x, int* y) {
		*x = m_posX;
		*y = m_posY;
	}

	void SetIsLived(bool b) {
		m_isLived = b;
	}
	bool GetIsLived() const {
		return m_isLived;
	}

	void CheckCollision(Object& other) {
		int x, y;
		GetPos(&x, &y);

		int otherX, otherY;
		other.GetPos(&otherX, &otherY);

		if (x == otherX && y == otherY) {
			other.CollidideWithType(m_type);
			SetIsLived(false);
		}
	}
	void SetType(ObjectType type) {
		m_type = type;
	}

private:
	int m_posX;
	int m_posY;
	ObjectType m_type;

	bool m_isLived = true;
};