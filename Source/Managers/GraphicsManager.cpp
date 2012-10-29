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
  mScreen->Draw(mSurfaces);
}

void GraphicsManager::SendMessage(Message const &aMessage)
{
}

Surface *GraphicsManager::CreateSurface()
{
#if !defined(ANDROID) && !defined(IOS)
	PCSurface *surface = new PCSurface();
#else
	Surface *surface = new Surface();
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
  mSurfaces.push_back(aSurface);
}

void GraphicsManager::RemoveSurface(Surface *aSurface)
{
  for(std::vector<Surface *>::iterator it = mSurfaces.begin(); it != mSurfaces.end(); ++it)
	{
		if(*it == aSurface)
		{
			mSurfaces.erase(it);
			break;
		}
	}
}
