#include "GameApp.h"
#include "TextParser.h"

int main()
{
	gGameApp = new GameApp();
	while(1)
	{
		gGameApp->Update();
	}
	delete gGameApp;

	return 0;
}
