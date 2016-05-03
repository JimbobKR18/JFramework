#ifndef __JFramework_GraphicsManager_H_
#define __JFramework_GraphicsManager_H_

#include "Manager.h"
#include "Surface.h"
#include "Screen.h"
#include "TextureData.h"
#include "ShaderData.h"

class GraphicsManager : public Manager
{
private:
  std::vector<Surface*>               mSurfaces;
  std::vector<Surface*>               mUIElements;
  std::map<HashString, TextureData>   mTextures;
  std::map<HashString, ShaderData>    mShaders;
  Screen*                             mScreen;

  static unsigned const sUID;
public:
  typedef std::vector<Surface*>::iterator SurfaceIT;

  GraphicsManager(GameApp *aApp, int aWidth, int aHeight);
  ~GraphicsManager();

  // Derived from Manager
  virtual void        Update();
  virtual void        SendMessage(Message const &aMessage);
  virtual void        ProcessDelayedMessage(Message *aMessage);
  static std::string  GetName() {return "GraphicsManager";}
  static unsigned     GetUID() {return sUID;}
  
  static void         SerializeLUA();

  // Manager stuff
  Surface*            CreateSurface();
  Surface*            CreateUISurface();
  void                DeleteSurface(Surface *aSurface);
  void                AddSurface(Surface *aSurface);
  void                AddUISurface(Surface *aSurface);
  void                RemoveSurface(Surface *aSurface);
  void                ClearSurfaces();

  // Get the screen info
  Screen*             GetScreen();
  
  // Texture management
  void                AddTexturePairing(HashString const &aFilename, TextureData const &aData);
  TextureData const&  GetTextureData(HashString const &aFilename) const;
  
  // Shader management
  void                AddShaderPairing(HashString const &aFilename, ShaderData const &aData);
  ShaderData const&   GetShaderData(HashString const &aFilename) const;
  bool                ShaderDataExists(HashString const &aFilename) const;

  // Misc.
  Vector3             AbsToRel(Vector3 const &aPosition) const;
  Vector3             RelToAbs(Vector3 const &aPosition) const;
};

#endif
