#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"
#include "View.h"

class Screen
{
private:
  int   mWidth,
        mHeight;
  View  mView;

public:
  Screen();
  Screen(int aW, int aH);
  virtual ~Screen();

  // Width and Height
  int                     GetWidth() const;
  int                     GetHeight() const;
  // Get viewable space
  View&                   GetView();
  // Batching
  void                    SortObjects(std::vector<Surface*> &aObjects);

  virtual void            ChangeSize(int aW, int aH, bool aFullScreen) = 0;
  virtual void            PreDraw() = 0;
  virtual void            Draw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            DrawUI(std::vector<Surface*> const &aObjects) = 0;
  virtual void            DebugDraw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            SwapBuffers() = 0;
};

#endif
