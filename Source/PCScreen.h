#ifndef __JFramework_PCScreen_h_
#define __JFramework_PCScreen_h_

#include "Screen.h"
#include <GL/gl.h>
#include <GL/glext.h>

class PCScreen : public Screen
{
  private:
    SDL_Surface *mSurface;

  public:
    PCScreen();
    PCScreen(int aW, int aH);
    virtual ~PCScreen();

    void Draw();
    void ChangeSize(int aW, int aH);
};

#endif
