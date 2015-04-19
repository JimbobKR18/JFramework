#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"
#include "View.h"
#include "ScreenRenderSorter.h"

class Screen
{
private:
  int                 mWidth;
  int                 mHeight;
  View                mView;
  ScreenRenderSorter* mBatchRenderSorter;
  ScreenRenderSorter* mDepthRenderSorter;

public:
  Screen();
  Screen(int aW, int aH);
  virtual ~Screen();

  // Width and Height
  int                     GetWidth() const;
  int                     GetHeight() const;
  // Get viewable space
  View&                   GetView();
  // Get batch render sorting method
  ScreenRenderSorter*     GetBatchRenderSorter();
  // Set batch render sorting method
  void                    SetBatchRenderSorter(ScreenRenderSorter *aBatchRenderSorter);
  // Get depth render sorting method (Post batching)
  ScreenRenderSorter*     GetDepthRenderSorter();
  // Set depth render sorting method
  void                    SetDepthRenderSorter(ScreenRenderSorter *aDepthRenderSorter);
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
