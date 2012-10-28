#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"

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
    virtual void Draw(std::vector<Surface*> const &aObjects) = 0;
};

#endif
