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

/**
 * @brief Update screen, swap buffers, etc.
 */
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

/**
 * @brief Does nothing
 * @param aMessage
 */
void GraphicsManager::SendMessage(Message const &aMessage)
{
}

/**
 * @brief Stores a message to be processed next frame.
 * @param aMessage Message to store
 */
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

/**
 * @brief Create a new surface with owner this.
 */
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

/**
 * @brief Create surface at UI layer, handy helper.
 */
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

/**
 * @brief Delete and remove surface.
 * @param aSurface
 */
void GraphicsManager::DeleteSurface(Surface *aSurface)
{
  RemoveSurface(aSurface);
  delete aSurface;
}

/**
 * @brief Add surface to list.
 * @param aSurface
 */
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

/**
 * @brief Add surface to UI list.
 * @param aSurface
 */
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

/**
 * @brief Remove surface, do not delete.
 * @param aSurface
 */
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

/**
 * @brief Remove and delete all surfaces
 */
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

/**
 * @brief Get screen object (the renderer)
 */
Screen *GraphicsManager::GetScreen()
{
  return mScreen;
}

/**
 * @brief Add texture name and data to map.
 * @param aFilename
 * @param aData
 */
void GraphicsManager::AddTexturePairing(std::string const &aFilename, TextureData const &aData)
{
  mTextures.insert(std::pair<std::string, TextureData>(aFilename, aData));
}

/**
 * @brief Get id for texture by filename
 * @param aFilename
 */
unsigned GraphicsManager::GetTextureID(std::string const &aFilename) const
{
  std::map<std::string, TextureData>::const_iterator pos = mTextures.find(aFilename);
  
  if(pos == mTextures.end())
  {
    return -1;
  }
  
  return pos->second.mTextureID;
}

/**
 * @brief Get data for texture by filename
 * @param aFilename
 */
TextureData const& GraphicsManager::GetTextureData(std::string const &aFilename) const
{
  std::map<std::string, TextureData>::const_iterator pos = mTextures.find(aFilename);

  if(pos == mTextures.end())
  {
    return mTextures.find(DEFAULT_TEXTURE_NAME)->second;
  }

  return pos->second;
}

/**
 * @brief Converts absolute coordinate point to point in local screen space.
 * @param aPosition
 */
Vector3 GraphicsManager::AbsToRel(Vector3 const &aPosition) const
{
  Vector3 ret = mScreen->GetView().GetPosition();

  ret -= mScreen->GetView().GetSize()/2;
  ret += aPosition;

  return ret;
}

/**
 * @brief Converts local coordinate point to point in world space.
 * @param aPosition
 */
Vector3 GraphicsManager::RelToAbs(Vector3 const &aPosition) const
{
  // TODO test this sometime
  Vector3 ret = aPosition;

  ret += mScreen->GetView().GetSize()/2;
  ret -= mScreen->GetView().GetPosition();

  return ret;
}
