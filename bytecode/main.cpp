#include "stdafx.h"
#include "read_write_file.h"
#include "virtual_machine.h"

int main()
{
	// ����Ʈ�ڵ� ����
	char code[] = {
		0x00, 0x01, 0x02, 0x03, 0x04
	};
	WriteFileInBinary("bytecode.bin", code, sizeof(code) / sizeof(char));
	
	// ����Ʈ�ڵ� �̿�
	char* data = ReadFileInBinary("bytecode.bin");

	VirtualMachine* vm = new VirtualMachine();
	vm->Interpret(data, sizeof(data) / sizeof(char));

	delete data;
	delete vm;
}
