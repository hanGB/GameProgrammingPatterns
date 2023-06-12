#pragma once

class PerGame {
public:
	PerGame();
	~PerGame();

	void HandleInput(WPARAM wParam, bool isDown);
	void Update(int deltaTime);
	void Render(HWND m_hWnd);

private:
	void UpdateWorld(double deltaTime);
	void RenderWorld(HDC memDC);

	CoordinateData m_coordinateData;
	int m_updateLag = 0;

	bool m_flag = false;
};