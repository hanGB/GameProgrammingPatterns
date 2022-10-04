#include "stdafx.h"
#include "child_of_file_system.h"
#include "one_instance_class.h"
#include "game.h"

int main()
{
	char path[] = "path";
	char contents[] = "contents";
	FileSystem::Instance().ReadFile(path);
	FileSystem::Instance().WriteFile(path, contents);

	OneInstanceClass oic = OneInstanceClass();
	//OneInstanceClass oic = OneInstanceClass();

	Character chara = Character();
	Character chara1 = Character();
	Character chara2 = Character();
	chara.GiveSomeImformation();
	chara1.GiveSomeImformation();
	chara2.GiveSomeImformation();

	Game::Instatnce().SetLog(*new Log());
	Game::Instatnce().SetAudioPlayer(*new AudioPlayer());

	Game::Instatnce().GetLog().WriteLog("It's end!");
	Game::Instatnce().GetAudioPlayer().Play("Music");
}