#ifndef __JFramework_PCScreen_h_
#define __JFramework_PCScreen_h_

#include "Screen.h"
#if defined(_WIN32)
  #include <Windows.h>
  #include <gl/gl.h>
  #include "SDL.h"
  #undef SendMessage
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  #include <SDL/SDL.h>
#else
  #include <GL/gl.h>
  #include <GL/glext.h>
  #include <SDL/SDL.h>
#endif

class PCScreen : public Screen
{
  private:
    SDL_Surface* mSurface;

  public:
    PCScreen();
    PCScreen(int aW, int aH);
    virtual ~PCScreen();

    void Draw(std::vector<Surface*> const &aObjects);
    void ChangeSize(int aW, int aH);
};

#endif
