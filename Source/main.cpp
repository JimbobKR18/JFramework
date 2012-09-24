#include "GameApp.h"

int main(int argc, char *argv[])
{
	gGameApp = new GameApp();
	while(1)
	{
		gGameApp->Update();
	}
	delete gGameApp;

	return 0;
}
