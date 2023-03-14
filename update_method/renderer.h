#pragma once
#include "stdafx.h"

class Renderer {
public:
	Renderer()
	{
		m_screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, NULL);
		m_screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, NULL);
		m_screenBufferIndex = 0;
	}
	~Renderer()
	{
		CloseHandle(m_screenBuffer[0]);
		CloseHandle(m_screenBuffer[1]);
	}
	void PrintOnBuffer(int x, int y, const char* text)
	{
		// world coord
		COORD pos = { (short)(x + WORLD_WIDTH), (short)(-y + WORLD_HEIGHT) };
		SetConsoleCursorPosition(m_screenBuffer[m_screenBufferIndex], pos);

		DWORD dw;
		WriteFile(m_screenBuffer[m_screenBufferIndex], text, (DWORD)strlen(text), &dw, nullptr);
	}
	void PrintOnBuffer(int x, int y, char* text)
	{
		// world coord
		COORD pos = { (short)(x + WORLD_WIDTH), (short)(-y + WORLD_HEIGHT) };
		SetConsoleCursorPosition(m_screenBuffer[m_screenBufferIndex], pos);

		DWORD dw;
		WriteFile(m_screenBuffer[m_screenBufferIndex], text, (DWORD)strlen(text), &dw, nullptr);
	}
	void PrintOnBuffer(double x, double y, char* text)
	{
		// world coord
		COORD pos = { (short)(std::round(x) + WORLD_WIDTH), (short)(-std::round(y) + WORLD_HEIGHT) };
		SetConsoleCursorPosition(m_screenBuffer[m_screenBufferIndex], pos);

		DWORD dw;
		WriteFile(m_screenBuffer[m_screenBufferIndex], text, (DWORD)strlen(text), &dw, nullptr);
	}
	void FlipBuffer()
	{
		SetConsoleActiveScreenBuffer(m_screenBuffer[m_screenBufferIndex]);
		m_screenBufferIndex = (m_screenBufferIndex + 1) % 2;
		ClearBuffer();
	}

private:
	void ClearBuffer()
	{
		COORD coord = { 0, 0 };
		DWORD dw;
		FillConsoleOutputCharacter(m_screenBuffer[m_screenBufferIndex], ' ', 100 * 100, coord, &dw);
	}

	HANDLE	m_screenBuffer[2];
	int		m_screenBufferIndex;
};