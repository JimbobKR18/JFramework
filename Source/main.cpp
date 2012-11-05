#include "GameApp.h"
#include "TextParser.h"

#ifdef _WIN32
  #include "SDL.h"
#endif

int main(int argc, char *argv[])
{
	GameApp *app = new GameApp();
	while(1)
	{
		app->Update();
	}
	delete app;

	return 0;
}
