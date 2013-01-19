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
  int GetWidth() const;
  int GetHeight() const;

  // Get viewable space
  View &GetView();

  // Batching
  std::vector<Surface*> SortObjects(std::vector<Surface*> const &aObjects);

  virtual void ChangeSize(int aW, int aH) = 0;
  virtual void Draw(std::vector<Surface*> const &aObjects) = 0;
};

#endif
