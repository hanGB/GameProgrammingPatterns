#pragma once
#include "stdafx.h"
#include "frame_buffer.h"

class Scene {
public:
	Scene() : m_current(&m_buffer[0]), m_next(&m_buffer[1]) {}
	~Scene() {}

	void Draw() {
		m_next->Clear();
		m_next->Draw(1, 1);	m_next->Draw(4, 1);
		m_next->Draw(1, 3);	m_next->Draw(2, 4);
		m_next->Draw(3, 4);	m_next->Draw(4, 3);
		Swap();
	}

	FrameBuffer& GetBuffer() {
		return *m_current;
	}

private:
	void Swap() {
		FrameBuffer* temp = m_current;
		m_current = m_next;
		m_next = temp;
	}

	FrameBuffer m_buffer[2];
	FrameBuffer* m_current;
	FrameBuffer* m_next;
};