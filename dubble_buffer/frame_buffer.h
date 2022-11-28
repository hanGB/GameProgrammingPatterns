#pragma once
#include "stdafx.h"

class FrameBuffer {
public:
	FrameBuffer() { Clear(); }
	~FrameBuffer() { }

	void Clear() {
		for (int i = 0; i < WIDTH * HEIGHT; ++i) {
			m_pixels[i] = 0;
		}
	}
	void Draw(int x, int y) {
		m_pixels[(WIDTH * y) + x] = CHAR_MAX;
	}
	const char* GetPixels() {
		return m_pixels;
	}

private:
	static const int WIDTH = WINDOW_SIZE / PIXEL_SIZE;
	static const int HEIGHT = WINDOW_SIZE / PIXEL_SIZE;

	char m_pixels[WIDTH * HEIGHT];
};