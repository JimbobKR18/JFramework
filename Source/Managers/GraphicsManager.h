#ifndef __JFramework_GraphicsManager_H_
#define __JFramework_GraphicsManager_H_

#include "Manager.h"
#include "Camera.h"
#include "Screen.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "QuadTree.h"
#include "Renderable.h"
#include "Pipeline.h"

class GraphicsManager : public Manager
{
public:
  typedef std::unordered_set<Renderable*>::iterator RenderableIT;
  
private:
  Tree*                                 mTree;
  std::unordered_set<Renderable*>       mRenderables;
  std::unordered_set<Renderable*>       mNewRenderables;
  std::unordered_set<Renderable*>       mMovingRenderables;
  std::unordered_set<Renderable*>       mUIElements;
  std::unordered_map<int, TextureData*>  mTextures;
  std::unordered_map<int, ShaderData*>   mShaders;
  std::unordered_set<Camera*>         mCameras;
  std::unordered_set<int>             mUnsortedLayers;
  std::unordered_map<int, Pipeline*>   mPipelines;
  Screen*                             mScreen;
  Camera*                             mPrimaryCamera;
  ThreadPool                          mThreadPool;

  static unsigned const sUID;
public:
  GraphicsManager(GameApp *aApp, int aWidth, int aHeight, bool aFullScreen);
  GraphicsManager(GraphicsManager const &aGraphicsManager);
  ~GraphicsManager();

  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "GraphicsManager";}
  static unsigned     GetUID() {return sUID;}
  
  static void         SerializeLUA();

  // Renderable Management
  Renderable*         CreateRenderable(HashString const &aType);
  Renderable*         CreateUIRenderable(HashString const &aType);
  void                DeleteRenderable(Renderable *aRenderable);
  void                AddRenderable(Renderable *aRenderable);
  void                AddUIRenderable(Renderable *aRenderable);
  void                RemoveRenderable(Renderable *aRenderable);
  void                ClearRenderables();
  
  // Camera Management
  Camera*             CreateCamera();
  void                DeleteCamera(Camera *aCamera);
  void                AddCamera(Camera *aCamera);
  void                RemoveCamera(Camera *aCamera);
  void                ClearCameras();
  void                SetPrimaryCamera(Camera *aCamera);
  
  // Pipeline Management
  Pipeline*           CreatePipeline(HashString const &aFileName);
  Pipeline*           GetPipeline(int const &aId);
  
  // Layer Management
  void                SetUnsortedLayers(std::unordered_set<int> const aUnsortedLayers);

  // Getters
  Screen*             GetScreen();
  Camera*             GetPrimaryCamera();
  Tree*               GetTree();
  
  // Texture management
  void                AddTexturePairing(HashString const &aFilename, TextureData *aData);
  TextureData*        GetTextureData(HashString const &aFilename) const;
  
  // Shader management
  void                AddShaderPairing(HashString const &aFilename, ShaderData *aData);
  ShaderData*         GetShaderData(HashString const &aFilename) const;
  bool                ShaderDataExists(HashString const &aFilename) const;
  void                SetGlobalShaderProperty(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename,
                                              RenderableProperty const &aProperty);

  // Misc.
  void                ResetDevice();
  void                ResizeTree(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  void                ClearTree();
  Vector3             AbsToRel(Vector3 const &aPosition) const;
  Vector3             RelToAbs(Vector3 const &aPosition) const;
};

#endif
