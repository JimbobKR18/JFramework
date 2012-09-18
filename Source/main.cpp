#include "GameApp.h"

int main(int argc, char *argv[])
{
	GameApp *gameApp = new GameApp();
	while(1)
	{
		gameApp->Update();
	}
	delete gameApp;

	return 0;
}
