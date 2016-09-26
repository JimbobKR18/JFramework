#include "Common.h"
#include "GraphicsManager.h"
#include "ObjectDeleteMessage.h"

#if !defined(__APPLE__) && !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
#endif

#ifdef SHADER_COMPATIBLE
  #include "PCShaderScreen.h"
  #include "PCShaderSurface.h"
#else
  #include "PCScreen.h"
  #include "PCSurface.h"
#endif

#define DEFAULT_TEXTURE_NAME "DefaultEmptyFirstBlank"

unsigned const GraphicsManager::sUID = Common::StringHashFunction("GraphicsManager");
GraphicsManager::GraphicsManager(GameApp *aApp, int aWidth, int aHeight, bool aFullScreen) : Manager(aApp, "GraphicsManager", GraphicsManager::sUID),
                                                                           mSurfaces(), mUIElements(), mTextures(), mShaders(), mScreen(nullptr), mProperties()
{
  // Add Default Texture
  AddTexturePairing(DEFAULT_TEXTURE_NAME, TextureData(-1, 0, 0));
#ifndef SHADER_COMPATIBLE
  mScreen = new PCScreen(aWidth, aHeight, aFullScreen);
#else
  mScreen = new PCShaderScreen(aWidth, aHeight, aFullScreen);
#endif
}

GraphicsManager::~GraphicsManager()
{
  for(PropertyMapIt it = mProperties.begin(); it != mProperties.end(); ++it)
  {
    for(PropertyContainerIt it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    {
      delete *it2;
    }
  }
  mProperties.clear();
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
 * @brief Make this manager visible in LUA.
 */
void GraphicsManager::SerializeLUA()
{
  SLB::Class<GraphicsManager>("GraphicsManager").inherits<Manager>()
          .set("AddOrEditProperty", &GraphicsManager::AddOrEditProperty)
          .set("ClearProperties", &GraphicsManager::ClearProperties);
}

/**
 * @brief Create a new surface with owner this.
 */
Surface *GraphicsManager::CreateSurface()
{
#ifndef SHADER_COMPATIBLE
  Surface *surface = new PCSurface(this);
#else
	Surface *surface = new PCShaderSurface(this);
#endif

	AddSurface(surface);

	return surface;
}

/**
 * @brief Create surface at UI layer, handy helper.
 */
Surface *GraphicsManager::CreateUISurface()
{
#ifndef SHADER_COMPATIBLE
  Surface *surface = new PCSurface(this);
#else
  Surface *surface = new PCShaderSurface(this);
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
  ClearProperties(aSurface);
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
void GraphicsManager::AddTexturePairing(HashString const &aFilename, TextureData const &aData)
{
  mTextures.insert(std::pair<HashString, TextureData>(aFilename, aData));
}

/**
 * @brief Get data for texture by filename
 * @param aFilename
 */
TextureData const& GraphicsManager::GetTextureData(HashString const &aFilename) const
{
  std::map<HashString, TextureData>::const_iterator pos = mTextures.find(aFilename);

  if(pos == mTextures.end())
  {
    return mTextures.find(DEFAULT_TEXTURE_NAME)->second;
  }

  return pos->second;
}

/**
 * @brief Add shader pairing by name and data.
 * @param aFilename Name of shader file.
 * @param aData Data of shader file.
 */
void GraphicsManager::AddShaderPairing(HashString const &aFilename, ShaderData const &aData)
{
  mShaders.insert(std::pair<HashString, ShaderData>(aFilename, aData));
}

/**
 * @brief Get data of shader based on file name.
 * @param aFilename Name of shader file.
 * @return Data of shader file. Will assert if file not found.
 */
ShaderData const& GraphicsManager::GetShaderData(HashString const &aFilename) const
{
  std::map<HashString, ShaderData>::const_iterator pos = mShaders.find(aFilename);

  if(pos == mShaders.end())
  {
    DebugLogPrint("No shader with name %s found\n", aFilename.ToCharArray());
    assert(!"No shader found.");
  }

  return pos->second;
}

/**
 * @brief Get if shader already exists.
 * @param aFilename Name of shader file.
 * @return False if not found, true otherwise.
 */
bool GraphicsManager::ShaderDataExists(HashString const &aFilename) const
{
  std::map<HashString, ShaderData>::const_iterator pos = mShaders.find(aFilename);

  if(pos == mShaders.end())
  {
    return false;
  }

  return true;
}

/**
 * @brief Get property map.
 * @return Properties.
 */
GraphicsManager::PropertyMap& GraphicsManager::GetPropertyMap()
{
  return mProperties;
}

/**
 * @brief Edit property if there, otherwise add.
 * @param aName Name of property
 * @param aType Type of property
 * @param aTargetValue Target value of property
 * @param aDefaultValue Default value of property
 */
void GraphicsManager::AddOrEditProperty(Surface *aSurface, HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue)
{
  size_t id = (size_t)aSurface;
  PropertyContainer &properties = mProperties[id];
  PropertyContainerIt propertyEnd = properties.end();
  for(PropertyContainerIt it = properties.begin(); it != propertyEnd; ++it)
  {
    if((*it)->GetName() == aName)
    {
      (*it)->SetType(aType);
      (*it)->SetTargetValue(aTargetValue);
      (*it)->SetDefaultValue(aDefaultValue);
      return;
    }
  }
  properties.push_back(new SurfaceProperty(aName, aType, aTargetValue, aDefaultValue));
}

/**
 * @brief Clear properties for a surface.
 * @param aSurface Surface to clear.
 */
void GraphicsManager::ClearProperties(Surface *aSurface)
{
  size_t id = (size_t)aSurface;
  PropertyContainer &properties = mProperties[id];
  PropertyContainerIt propertyEnd = properties.end();
  for(PropertyContainerIt it = properties.begin(); it != propertyEnd; ++it)
  {
    delete *it;
  }
  properties.clear();
}

/**
 * @brief Converts absolute coordinate point to point in local screen space.
 * @param aPosition
 */
Vector3 GraphicsManager::AbsToRel(Vector3 const &aPosition) const
{
  Vector3 ret = mScreen->GetView().GetPosition().Multiply(mScreen->GetView().GetScale());

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
  Vector3 ret = aPosition;

  ret += mScreen->GetView().GetSize() / 2.0f;
  ret -= mScreen->GetView().GetPosition().Multiply(mScreen->GetView().GetScale());

  return ret;
}
