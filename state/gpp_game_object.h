#pragma once
#include "stdafx.h"

class GPPWorld;

enum class GPPGameObjectType {
	PLAYER,
	FIXED,
	MOVABLE
};

class GPPGameObject {
public:
	GPPGameObject() {
		SetVelocity(0.0f, 0.0f);
	}
	GPPGameObject(float positionX, float positionY, float sizeX, float sizeY, int red, int green, int blue) {

		SetPosition(positionX, positionY);
		SetHalfSize(sizeX / 2.0f, sizeY / 2.0f);
		SetRGBColor(red, green, blue);

		SetVelocity(0.0f, 0.0f);
	}
	~GPPGameObject() {}

	virtual void HandleInput(GPPInputChunk& inputs) {};
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(HDC& memDC, float posInWindowX, float posInWindowY) = 0;
	void Render(HDC& memDC, GPPCoordinateData& coordinateData) {

		HPEN hPen, oldPen;
		HBRUSH hBrush, oldBrush;

		hPen = CreatePen(PS_SOLID, 0, RGB(m_rgbColor[0], m_rgbColor[1], m_rgbColor[2]));
		hBrush = CreateSolidBrush(RGB(m_rgbColor[0], m_rgbColor[1], m_rgbColor[2]));
		oldPen = (HPEN)SelectObject(memDC, hPen);
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush);

		float posInWindowX = m_positionX;
		float posInWindowY = m_positionY;
		coordinateData.ConvertCoordinateWindowToOpenGL(&posInWindowX, &posInWindowY);

		Render(memDC, posInWindowX, posInWindowY);

		(HPEN)SelectObject(memDC, oldPen);
		(HBRUSH)SelectObject(memDC, oldBrush);

		DeleteObject(oldPen);
		DeleteObject(oldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
	void LandOnGround() {
		m_isFalling = false;
		m_velocityY = 0.0f;
	}
	void Fall() {
		m_isFalling = true;
	}


	void SetType(GPPGameObjectType type) {
		m_type = type;
	}
	void SetPosition(float x, float y) {
		m_positionX = x;
		m_positionY = y;
	}
	void SetHalfSize(float x, float y) {
		m_halfSizeX = x;
		m_halfSizeY = y;
	}
	void SetVelocity(float x, float y) {
		m_velocityX = x;
		m_velocityY = y;
	}
	void SetMass(float mass) {
		m_mass = mass;
	}
	void SetRGBColor(int r, int g, int b) {
		m_rgbColor[0] = r;
		m_rgbColor[1] = g;
		m_rgbColor[2] = b;
	}
	GPPGameObjectType GetType() const {
		return m_type;
	}

	void SetGPPWorld(GPPWorld& world);

	void SetLifeTime(float life) {
		m_lifeTime = life;
		m_isUseLifeTime = true;
	}
	void SetIsCollided(bool collided) {
		m_isCollided = collided;
	}
	void SetIsFalling(bool falling) {
		m_isFalling = falling;
	}

	void GetPosition(float* x, float* y) {
		*x = m_positionX;
		*y = m_positionY;
	}
	void GetHalfSize(float* x, float* y) {
		*x = m_halfSizeX;
		*y = m_halfSizeY;
	}
	void GetVelocity(float* x, float* y) {
		*x = m_velocityX;
		*y = m_velocityY;
	}
	void GetMass(float* mass) {
		*mass = m_mass;
	}
	void GetRGBColor(int* r, int* g, int* b) {
		*r = m_rgbColor[0];
		*g = m_rgbColor[1];
		*b = m_rgbColor[2];
	}
	bool GetIsUseLifeTime() const {
		return m_isUseLifeTime;
	}
	float GetLifeTime() const {
		return m_lifeTime;
	}
	bool GetIsCollided() const {
		return m_isCollided;
	}
	bool GetIsFalling() const {
		return m_isFalling;
	}
	GPPWorld* GetGPPWorld();


protected:
	float m_positionX, m_positionY;
	float m_halfSizeX, m_halfSizeY;
	float m_velocityX, m_velocityY;
	float m_mass;

	int m_rgbColor[3];
	bool m_isFalling;

	GPPWorld* m_world;

private:
	GPPGameObjectType m_type = GPPGameObjectType::MOVABLE;
	bool m_isUseLifeTime = false;
	float m_lifeTime = 0.0f;
	bool m_isCollided = false;

};
