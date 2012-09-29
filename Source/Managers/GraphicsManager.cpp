#include "Common.h"
#include "GraphicsManager.h"

#if !defined(IOS) && !defined(ANDROID)
  #include "PCScreen.h"
#endif

GraphicsManager::GraphicsManager() : Manager("GraphicsManager")
{
#if !defined(IOS) && !defined(ANDROID)
  mScreen = new PCScreen(640, 480);
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
	Surface *surface = new Surface();

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
