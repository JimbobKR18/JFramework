#include "GameApp.h"
#include "InputManager.h"
#include "DebugManager.h"

#if defined(_WIN32) || defined(__APPLE__)
  #include "SDL.h"
#else
  #include <SDL/SDL.h>
#endif

//#define _DEBUG

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
#ifdef _DEBUG
    if(app->GET<DebugManager>()->HandleEvent(event))
      continue;
#endif

    switch(event.type)
    {
      case SDL_QUIT:
        exit(0);
        break;
      case SDL_USEREVENT:
        app->Update();
        break;
      case SDL_MOUSEBUTTONDOWN:
        app->GET<InputManager>()->AddInput("Mouse", Vector3(event.button.x,event.button.y,0));
        break;
      case SDL_MOUSEBUTTONUP:
        app->GET<InputManager>()->RemoveInput("Mouse");
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
          case SDLK_LEFT:
          case SDLK_a:
            app->GET<InputManager>()->AddInput("MoveLeft", Vector3(0,0,0));
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            app->GET<InputManager>()->AddInput("MoveRight", Vector3(0,0,0));
            break;
          case SDLK_UP:
          case SDLK_w:
            app->GET<InputManager>()->AddInput("MoveUp", Vector3(0,0,0));
            break;
          case SDLK_DOWN:
          case SDLK_s:
            app->GET<InputManager>()->AddInput("MoveDown", Vector3(0,0,0));
            break;
          case SDLK_q:
            app->GET<InputManager>()->AddInput("QPressed", Vector3(0,0,0));
            break;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
          case SDLK_LEFT:
          case SDLK_a:
            app->GET<InputManager>()->RemoveInput("MoveLeft");
            break;
          case SDLK_RIGHT:
          case SDLK_d:
            app->GET<InputManager>()->RemoveInput("MoveRight");
            break;
          case SDLK_UP:
          case SDLK_w:
            app->GET<InputManager>()->RemoveInput("MoveUp");
            break;
          case SDLK_DOWN:
          case SDLK_s:
            app->GET<InputManager>()->RemoveInput("MoveDown");
            break;
          case SDLK_q:
            app->GET<InputManager>()->RemoveInput("QPressed");
            break;
          default:
            break;
        }
        break;
    }
  }
  delete app;

  return 0;
}
