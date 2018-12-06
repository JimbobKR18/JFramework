#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Surface.h"
#include "ScreenRenderSorter.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "Camera.h"
#include "Transform.h"
#include "QuadTree.h"

class Screen
{
private:
  GraphicsManager*    mOwner;
  int                 mWidth;
  int                 mHeight;
  bool                mFullScreen;
  ScreenRenderSorter* mBatchRenderSorter;
  ScreenRenderSorter* mDepthRenderSorter;
  ScreenRenderSorter* mUIRenderSorter;

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
  // Get batch render sorting method
  ScreenRenderSorter*     GetBatchRenderSorter();
  // Set batch render sorting method
  void                    SetBatchRenderSorter(ScreenRenderSorter *aBatchRenderSorter);
  // Get depth render sorting method (Post batching)
  ScreenRenderSorter*     GetDepthRenderSorter();
  // Set depth render sorting method
  void                    SetDepthRenderSorter(ScreenRenderSorter *aDepthRenderSorter);
  // Set UI render sorting method
  void                    SetUIRenderSorter(ScreenRenderSorter *aUIRenderSorter);
  // Get UI render sorting method (Post batching)
  ScreenRenderSorter*     GetUIRenderSorter();
  // Batching
  std::unordered_map<Camera*, std::map<int, std::vector<Surface*>>> PruneObjects(Tree *aObjects, 
                                       std::unordered_set<Surface*> const &aMovingObjects, std::unordered_set<Camera*> const &aCameras);
  void                    SortObjects(std::vector<Surface*> &aObjects);
  void                    SortUI(std::vector<Surface*> &aObjects);

  virtual void            ResetObjectTexture(Surface* aSurface, TextureData* aOldData, TextureData* aNewData) = 0;
  virtual void            ResetObjectShader(Surface* aSurface, ShaderData* aOldData, ShaderData* aNewData) = 0;
  virtual void            SetClearColor(Vector4 const &aClearColor) = 0;
  virtual void            ChangeSize(int aW, int aH, bool aFullScreen) = 0;
  virtual void            PreDraw() = 0;
  virtual void            Draw(std::map<int, std::vector<Surface*>> const &aObjects, Camera* aCamera) = 0;
  virtual void            DebugDraw(std::vector<Surface*> const &aObjects) = 0;
  virtual void            SwapBuffers() = 0;
  
protected:
  void                    AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition);
};

#endif
