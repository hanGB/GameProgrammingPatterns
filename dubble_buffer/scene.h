#pragma once
#include "stdafx.h"
#include "frame_buffer.h"

class Scene {
public:
	Scene() {}
	~Scene() {}

	void Draw() {
		m_buffer.Clear();
		m_buffer.Draw(1, 1);	m_buffer.Draw(4, 1);
		m_buffer.Draw(1, 3);	m_buffer.Draw(2, 4);
		m_buffer.Draw(3, 4);	m_buffer.Draw(4, 3);
	}

	FrameBuffer& GetBuffer() {
		return m_buffer;
	}

private:
	FrameBuffer m_buffer;
};