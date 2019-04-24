#ifndef __JFramework_Screen_h_
#define __JFramework_Screen_h_

#include "Renderable.h"
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
  // Get depth render sorting method (Post batching)
  ScreenRenderSorter*     GetDepthRenderSorter();
  // Set depth render sorting method
  void                    SetDepthRenderSorter(ScreenRenderSorter *aDepthRenderSorter);
  // Set UI render sorting method
  void                    SetUIRenderSorter(ScreenRenderSorter *aUIRenderSorter);
  // Get UI render sorting method (Post batching)
  ScreenRenderSorter*     GetUIRenderSorter();
  // Batching
  std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>> PruneObjects(Tree *aObjects, 
                                       std::unordered_set<Renderable*> const &aMovingObjects, std::unordered_set<Camera*> const &aCameras);
  void                    SortObjects(std::vector<Renderable*> &aObjects);
  void                    SortUI(std::vector<Renderable*> &aObjects);

  virtual void            SetGlobalShaderProperty(ShaderData *aShaderData, RenderableProperty const &aProperty) = 0;
  virtual void            ResetObjectTexture(Renderable* aRenderable, TextureData* aOldData, TextureData* aNewData) = 0;
  virtual void            ResetObjectShader(Renderable* aRenderable, ShaderData* aOldData, ShaderData* aNewData) = 0;
  virtual void            SetClearColor(Vector4 const &aClearColor) = 0;
  virtual void            ChangeSize(int aW, int aH, bool aFullScreen) = 0;
  virtual void            PreDraw() = 0;
  virtual void            Draw(std::map<int, std::vector<Renderable*>> const &aObjects, Camera* aCamera) = 0;
  virtual void            DebugDraw(std::vector<Renderable*> const &aObjects) = 0;
  virtual void            SwapBuffers() = 0;
};

#endif
