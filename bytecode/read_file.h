#pragma once
#include "stdafx.h"

char* ReadFile(const char* fileName)
{
	// ������ ���� ����
	std::fstream in;
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