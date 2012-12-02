#include "GameApp.h"
#include "TextParser.h"

#if defined(_WIN32) || defined(__APPLE__)
  #include "SDL.h"
#else
  #include <SDL/SDL.h>
#endif

Uint32 SDLUpdateTimer(Uint32 aInterval, void *aParam)
{
  SDL_Event event;
  
  event.type = SDL_USEREVENT;
  event.user.code = 1;
  event.user.data1 = 0;
  event.user.data2 = 0;
  
  SDL_PushEvent(&event);
  
  return aInterval;
}

int main(int argc, char *argv[])
{
  GameApp *app = new GameApp();
  SDL_Event event;
  SDL_AddTimer((1.0f/60.0f)*1000.0f, SDLUpdateTimer, app);
  
  while(SDL_WaitEvent(&event))
  {
    switch(event.type)
    {
      case SDL_USEREVENT:
        app->Update();
        break;
    }
  }
  delete app;

  return 0;
}
