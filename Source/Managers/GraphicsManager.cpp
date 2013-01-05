#include "Common.h"
#include "GraphicsManager.h"

#if !defined(IOS) && !defined(ANDROID)
  #include "PCScreen.h"
  #include "PCSurface.h"
#endif

GraphicsManager::GraphicsManager(GameApp *aApp) : Manager(aApp, "GraphicsManager")
{
#if !defined(IOS) && !defined(ANDROID)
  mScreen = new PCScreen(640, 480);
#else
#endif
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Update()
{
  mScreen->GetView().Update();
  std::vector<Surface*> sortedObjects = mScreen->SortObjects(mSurfaces);
  mScreen->Draw(sortedObjects);
}

void GraphicsManager::SendMessage(Message const &aMessage)
{
}

Surface *GraphicsManager::CreateSurface()
{
#if !defined(ANDROID) && !defined(IOS)
	PCSurface *surface = new PCSurface(this);
#else
	Surface *surface = new Surface(this);
#endif

	AddSurface(surface);

	return surface;
}

void GraphicsManager::DeleteSurface(Surface *aSurface)
{
  RemoveSurface(aSurface);
  delete aSurface;
}

void GraphicsManager::AddSurface(Surface *aSurface)
{
  // Check to see if object is in our list
  for(SurfaceIT it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
	{
		if(*it == aSurface)
		{
			return;
		}
	}
  
  mSurfaces.push_back(aSurface);
}

void GraphicsManager::RemoveSurface(Surface *aSurface)
{
  for(SurfaceIT it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
	{
		if(*it == aSurface)
		{
			mSurfaces.erase(it);
			break;
		}
	}
}

void GraphicsManager::ClearSurfaces()
{
  for(SurfaceIT it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
  {
    DeleteSurface(*it);
  }
}

Screen *GraphicsManager::GetScreen()
{
  return mScreen;
}

void GraphicsManager::AddTexturePairing(std::string const &aFilename, unsigned aTextureId)
{
  mTextures.insert(std::pair<std::string, unsigned>(aFilename, aTextureId));
}
unsigned GraphicsManager::GetTextureID(std::string const &aFilename)
{
  std::map<std::string, unsigned>::iterator pos = mTextures.find(aFilename);
  
  if(pos == mTextures.end())
  {
    return -1;
  }
  
  return pos->second;
}
