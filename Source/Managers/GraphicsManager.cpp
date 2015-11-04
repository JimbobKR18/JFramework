#include "Common.h"
#include "GraphicsManager.h"
#include "ObjectDeleteMessage.h"

#if !defined(IOS) && !defined(ANDROID)
  #include "PCScreen.h"
  #include "PCSurface.h"
#endif

#define DEFAULT_TEXTURE_NAME "DefaultEmptyFirstBlank"

unsigned const GraphicsManager::sUID = Common::StringHashFunction("GraphicsManager");
GraphicsManager::GraphicsManager(GameApp *aApp, int aWidth, int aHeight) : Manager(aApp, "GraphicsManager", GraphicsManager::sUID)
{
  // Add Default Texture
  AddTexturePairing(DEFAULT_TEXTURE_NAME, TextureData(-1, 0, 0));
#if !defined(IOS) && !defined(ANDROID)
  mScreen = new PCScreen(aWidth, aHeight);
#else
#endif
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Update()
{
  mScreen->GetView().Update();
  mScreen->SortObjects(mSurfaces);
  mScreen->PreDraw();
  mScreen->Draw(mSurfaces);
  mScreen->DrawUI(mUIElements);
#ifdef _DEBUG_DRAW
  // Displays bounds of objects with PhysicsObject
  mScreen->DebugDraw(mSurfaces);
#endif
  mScreen->SwapBuffers();
}

void GraphicsManager::SendMessage(Message const &aMessage)
{
}

void GraphicsManager::ProcessDelayedMessage(Message *aMessage)
{
  if(aMessage->GetDescription() == OBJECT_DELETE.ToCharArray()) 
  {
    ObjectDeleteMessage *msg = (ObjectDeleteMessage*)aMessage;
    // If this object was our view target, remember to unassociate.
    if(mScreen->GetView().GetTarget() == msg->mObject)
    {
      mScreen->GetView().SetTarget(nullptr);
      mScreen->GetView().SetTime(0.0f);
    }
    delete aMessage;
  }
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

Surface *GraphicsManager::CreateUISurface()
{
#if !defined(ANDROID) && !defined(IOS)
  PCSurface *surface = new PCSurface(this);
#else
  Surface *surface = new Surface(this);
#endif

  AddUISurface(surface);

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
  SurfaceIT end = mSurfaces.end();
  for(SurfaceIT it = mSurfaces.begin(); it != end; ++it)
  {
    if(*it == aSurface)
    {
      return;
    }
  }
  
  mSurfaces.push_back(aSurface);
}

void GraphicsManager::AddUISurface(Surface *aSurface)
{
  // Check to see if object is in our list
  SurfaceIT end = mUIElements.end();
  for(SurfaceIT it = mUIElements.begin(); it != end; ++it)
  {
    if(*it == aSurface)
    {
      return;
    }
  }

  mUIElements.push_back(aSurface);
}

void GraphicsManager::RemoveSurface(Surface *aSurface)
{
  SurfaceIT end = mSurfaces.end();
  for(SurfaceIT it = mSurfaces.begin(); it != end; ++it)
  {
    if(*it == aSurface)
    {
      mSurfaces.erase(it);
      break;
    }
  }

  end = mUIElements.end();
  for(SurfaceIT it = mUIElements.begin(); it != end; ++it)
  {
    if(*it == aSurface)
    {
      mUIElements.erase(it);
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
  for(SurfaceIT it = mUIElements.begin(); it != mUIElements.end(); ++it)
  {
    DeleteSurface(*it);
  }
}

Screen *GraphicsManager::GetScreen()
{
  return mScreen;
}

void GraphicsManager::AddTexturePairing(std::string const &aFilename, TextureData const &aData)
{
  mTextures.insert(std::pair<std::string, TextureData>(aFilename, aData));
}

unsigned GraphicsManager::GetTextureID(std::string const &aFilename) const
{
  std::map<std::string, TextureData>::const_iterator pos = mTextures.find(aFilename);
  
  if(pos == mTextures.end())
  {
    return -1;
  }
  
  return pos->second.mTextureID;
}

TextureData const& GraphicsManager::GetTextureData(std::string const &aFilename) const
{
  std::map<std::string, TextureData>::const_iterator pos = mTextures.find(aFilename);

  if(pos == mTextures.end())
  {
    return mTextures.find(DEFAULT_TEXTURE_NAME)->second;
  }

  return pos->second;
}

Vector3 GraphicsManager::AbsToRel(Vector3 const &aPosition) const
{
  Vector3 ret = mScreen->GetView().GetPosition();

  ret -= mScreen->GetView().GetSize()/2;
  ret += aPosition;

  return ret;
}

Vector3 GraphicsManager::RelToAbs(Vector3 const &aPosition) const
{
  // TODO test this sometime
  Vector3 ret = aPosition;

  ret += mScreen->GetView().GetSize()/2;
  ret -= mScreen->GetView().GetPosition();

  return ret;
}
