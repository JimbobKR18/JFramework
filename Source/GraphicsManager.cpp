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
  mScreen->Draw();
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
	delete aSurface;
}

void GraphicsManager::AddSurface(Surface *aSurface)
{
}

void GraphicsManager::RemoveSurface(Surface *aSurface)
{
}
