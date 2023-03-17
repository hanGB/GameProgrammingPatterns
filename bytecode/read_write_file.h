#pragma once
#include "stdafx.h"

char* ReadFile(const char* fileName)
{
	// ������ ���� ����
	std::ifstream in;
	in.open(fileName);

	// ���� ���� ���
	in.seekg(0, std::ios::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[length + 1];

	// ���� ��ŭ ���� �б�
	in.read(data, length);
	data[length] = '\0';

	in.close();

	return data;
}

char* ReadFileInBinary(const char* fileName, int& size)
{
	// ������ ���� ����
	std::ifstream in;
	in.open(fileName, std::ios::binary);

	// ���� ���� ���
	in.seekg(0, std::ios::end);
	size = (int)in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[size];

	// ���� ��ŭ ���� �б�
	in.read(data, size);

	in.close();

	return data;
}

void WriteFileInBinary(const char* fileName, char* data, int size)
{
	// ������ ���� ����
	std::ofstream out;
	out.open(fileName, std::ios::out | std::ios::binary);

	// ���Ͽ� ����
	out.write(data, size);

	out.close();
}
