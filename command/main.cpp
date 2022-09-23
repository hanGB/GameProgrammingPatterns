#include "stdafx.h"
#include "input_handler.h"
#include "child_of_command.h"
#include "game_actor.h"
#include "command_list.h"

int main() 
{
	InputHandler* inputHandler = new InputHandler();
	Unit* player = new Unit("Player", 5, 5);
	inputHandler->SetUint(*player);
	CommandList* commandList = new CommandList();

	player->PrintLocation();

	while (true) {
		Command* input = inputHandler->HandleInput();
		if (input) {
			input->Execute();
			commandList->AddCommand(input);
		}

		if (GetAsyncKeyState(KEY_INPUT_Y) & 0x0001) commandList->Redo();
		if (GetAsyncKeyState(KEY_INPUT_Z) & 0x0001) commandList->Undo();
		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}

	delete inputHandler;
	delete player;
	delete commandList;
}