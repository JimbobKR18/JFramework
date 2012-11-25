#include "GameApp.h"
#include "TextParser.h"

#if defined(_WIN32) || defined(__APPLE__)
  #include "SDL.h"
#endif

#ifdef __APPLE__
Uint32 SDLAppleUpdate(Uint32 aInterval, void *aParam)
{
  SDL_Event event;
  
  event.type = SDL_USEREVENT;
  event.user.code = 1;
  event.user.data1 = 0;
  event.user.data2 = 0;
  
  SDL_PushEvent(&event);
  
  return aInterval;
}
#endif

int main(int argc, char *argv[])
{
	GameApp *app = new GameApp();
#ifndef __APPLE__
	while(1)
	{
		app->Update();
	}
#else
  SDL_Event event;
  SDL_TimerID timer;
  timer = SDL_AddTimer(16, SDLAppleUpdate, app);
  
  while(SDL_WaitEvent(&event))
  {
    switch(event.type)
    {
      case SDL_USEREVENT:
        app->Update();
        break;
    }
  }
#endif
  delete app;

	return 0;
}
