#include "GameApp.h"
#include "TextParser.h"

int main()
{
	GameApp *app = new GameApp();
	while(1)
	{
		app->Update();
	}
	delete app;

	return 0;
}
