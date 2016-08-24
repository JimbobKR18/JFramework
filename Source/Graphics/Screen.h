#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"
#include "View.h"
#include "ScreenRenderSorter.h"
#include "SurfaceProperty.h"

class Screen
{
public:
  typedef std::vector<SurfaceProperty*> PropertyContainer;
  typedef std::unordered_map<int, PropertyContainer> PropertyMap;
  typedef PropertyContainer::iterator PropertyContainerIt;
  typedef PropertyMap::iterator PropertyMapIt;
  
private:
  int                 mWidth;
  int                 mHeight;
  bool                mFullScreen;
  View                mView;
  ScreenRenderSorter* mBatchRenderSorter;
  ScreenRenderSorter* mDepthRenderSorter;
  PropertyMap         mProperties;

public:
  Screen();
  Screen(int aW, int aH, bool aFullScreen);
  virtual ~Screen();

  // Width, Height, and Fullscreen
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
  // Properties
  PropertyMap&            GetPropertyMap();

  virtual void            ChangeSize(int aW, int aH, bool aFullScreen) = 0;
  virtual void            PreDraw() = 0;
  virtual void            Draw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            DrawUI(std::vector<Surface*> const &aObjects) = 0;
  virtual void            DebugDraw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            SwapBuffers() = 0;
  virtual void            AddOrEditProperty(Surface *aSurface, HashString const &aName, PropertyType const &aType, HashString const &aValue) = 0;
};

#endif
