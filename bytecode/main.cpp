#include "stdafx.h"
#include "read_write_file.h"
#include "virtual_machine.h"
#include "bytecode.h"

int main()
{
	// 바이트코드 생성
	char code[] = { 
		INST_LITERAL, 0,
		INST_LITERAL, 0, INST_GET_HEALTH,
		INST_LITERAL, 0, INST_GET_AGILITY,
		INST_LITERAL, 0, INST_GET_WISDOM,
		INST_ADD,
		INST_LITERAL, 2, INST_DIVI,
		INST_ADD,
		INST_SET_HEALTH
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
