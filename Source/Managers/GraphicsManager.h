#ifndef __JFramework_GraphicsManager_H_
#define __JFramework_GraphicsManager_H_

#include "Manager.h"
#include "Surface.h"
#include "Screen.h"

class GraphicsManager : public Manager
{
private:
	std::vector<Surface*>           mSurfaces;
  std::map<std::string, unsigned> mTextures;
	Screen*                         mScreen;
public:
	GraphicsManager(GameApp *aApp);
	~GraphicsManager();

  // Derived from Manager
	void                Update();
	void                SendMessage(Message const &aMessage);
	static std::string  GetName() {return "GraphicsManager";}

  // Manager stuff
	Surface*            CreateSurface();
	void                DeleteSurface(Surface *aSurface);
  void                AddSurface(Surface *aSurface);
	void                RemoveSurface(Surface *aSurface);
	void                ClearSurfaces();

  // Get the screen info
  Screen*             GetScreen();
  
  // Texture management
  void                AddTexturePairing(std::string const &aFilename, unsigned aTextureId);
  unsigned            GetTextureID(std::string const &aFilename);
public:
  typedef std::vector<Surface*>::iterator SurfaceIT;
};

#endif
