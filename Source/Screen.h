#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include <SDL/SDL.h>

class Screen
{
  private:
    int mWidth, mHeight;

  public:
    Screen();
    Screen(int aW, int aH);
    virtual ~Screen();

    int GetWidth();
    int GetHeight();

    virtual void ChangeSize(int aW, int aH) = 0;
    virtual void Draw() = 0;
};

#endif
