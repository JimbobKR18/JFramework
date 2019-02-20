#ifndef __JFramework_GraphicsManager_H_
#define __JFramework_GraphicsManager_H_

#include "Manager.h"
#include "Surface.h"
#include "Camera.h"
#include "Screen.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "QuadTree.h"

class GraphicsManager : public Manager
{
public:
  typedef std::unordered_set<Surface*>::iterator SurfaceIT;
  
private:
  Tree*                                 mTree;
  std::unordered_set<Surface*>           mSurfaces;
  std::unordered_set<Surface*>           mNewSurfaces;
  std::unordered_set<Surface*>           mMovingSurfaces;
  std::unordered_set<Surface*>           mUIElements;
  std::unordered_map<int, TextureData*>  mTextures;
  std::unordered_map<int, ShaderData*>   mShaders;
  std::unordered_set<Camera*>         mCameras;
  std::unordered_set<int>             mUnsortedLayers;
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

  // Surface Management
  Surface*            CreateSurface();
  Surface*            CreateUISurface();
  void                DeleteSurface(Surface *aSurface);
  void                AddSurface(Surface *aSurface);
  void                AddUISurface(Surface *aSurface);
  void                RemoveSurface(Surface *aSurface);
  void                ClearSurfaces();
  
  // Camera Management
  Camera*             CreateCamera();
  void                DeleteCamera(Camera *aCamera);
  void                AddCamera(Camera *aCamera);
  void                RemoveCamera(Camera *aCamera);
  void                ClearCameras();
  void                SetPrimaryCamera(Camera *aCamera);
  
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
                                              SurfaceProperty const &aProperty);

  // Misc.
  void                ResetDevice();
  void                ResizeTree(Vector3 const &aMinRange, Vector3 const &aMaxRange);
  void                ClearTree();
  Vector3             AbsToRel(Vector3 const &aPosition) const;
  Vector3             RelToAbs(Vector3 const &aPosition) const;
};

#endif
