#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"
#include "View.h"
#include "ScreenRenderSorter.h"

class Screen
{
private:
  GraphicsManager*    mOwner;
  int                 mWidth;
  int                 mHeight;
  bool                mFullScreen;
  View                mView;
  ScreenRenderSorter* mBatchRenderSorter;
  ScreenRenderSorter* mDepthRenderSorter;

public:
  Screen();
  Screen(GraphicsManager *aOwner, int aW, int aH, bool aFullScreen);
  virtual ~Screen();

  // Width, Height, and Fullscreen
  GraphicsManager*        GetOwner() const;
  int                     GetWidth() const;
  void                    SetWidth(int const aWidth);
  int                     GetHeight() const;
  void                    SetHeight(int const aHeight);
  bool                    IsFullScreen() const;
  void                    SetFullScreen(bool const aFullScreen);
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

  virtual void            SetClearColor(Vector4 const &aClearColor) = 0;
  virtual void            ChangeSize(int aW, int aH, bool aFullScreen) = 0;
  virtual void            PreDraw() = 0;
  virtual void            Draw(std::vector<Surface*> const &aObjects, std::vector<Surface*> const &aUIObjects) = 0;
  virtual void            DebugDraw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            SwapBuffers() = 0;
};

#endif
