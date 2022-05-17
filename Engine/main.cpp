#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Framework* myGame = new MyGame();
	
	myGame->Run();

	return 0;
}