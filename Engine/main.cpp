#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame myGame;

	myGame.Initialize();


	while (true)  // ゲームループ
	{
		myGame.Update();
		if (myGame.GetGameloopEndReqest() == true) {
			break;
		}
		myGame.Draw();
	}
	myGame.Finalize();
	return 0;
}