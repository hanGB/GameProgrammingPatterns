#pragma once
#include "stdafx.h"

class GPPGameObject {
public:
	GPPGameObject() {

		SetVelocity(0.0f, 0.0f);
	}
	~GPPGameObject() {}

	virtual void Update(float elapsedTime, GPPInputChunk& inputs) = 0;
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
	void SetType(GPPGameObjectType type) {
		m_type = type;
	}
	void SetState(GPPGameObjectState state) {
		m_state = state;
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
	GPPGameObjectState GetState() const {
		return m_state;
	}

	void GetPosition(float *x, float *y) {
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

protected:
	float m_positionX, m_positionY;
	float m_halfSizeX, m_halfSizeY;
	float m_velocityX, m_velocityY;
	float m_mass;

	int m_rgbColor[3];

	GPPGameObjectState m_state = GPPGameObjectState::STANDING_ON_GROUND;

private:
	GPPGameObjectType m_type = GPPGameObjectType::MOVABLE;
};
