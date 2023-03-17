#include "stdafx.h"
#include "read_write_file.h"
#include "virtual_machine.h"

int main()
{
	// 바이트코드 생성
	char code[] = {
		0x00, 0, 0x00, 10, 0x01, 
		0x00, 0, 0x00, 5, 0x02,
		0x00, 1, 0x00, 8, 0x03, 
		0x00, 0x00, 0x04,
		0x00, 0x00, 0x05
	};
	WriteFileInBinary("bytecode.bin", code, sizeof(code) / sizeof(char));
	
	// 바이트코드 이용
	int size;
	char* data = ReadFileInBinary("bytecode.bin", size);

	VirtualMachine* vm = new VirtualMachine();
	vm->Interpret(data, size);

	delete data;
	delete vm;
}
