#include "Common.h"
#include "GraphicsManager.h"
#include "ObjectDeleteMessage.h"
#include "ObjectManager.h"

#if !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
#endif

#ifdef SHADER_COMPATIBLE
  #include "PCShaderScreen.h"
  #include "PCShaderSurface.h"
  #include "ShaderLoader.h"
#else
  #include "PCScreen.h"
  #include "PCSurface.h"
#endif

#define DEFAULT_TEXTURE_NAME "DefaultEmptyFirstBlank"

unsigned const GraphicsManager::sUID = Common::StringHashFunction("GraphicsManager");
GraphicsManager::GraphicsManager(GameApp *aApp, int aWidth, int aHeight, bool aFullScreen) : Manager(aApp, "GraphicsManager", GraphicsManager::sUID),
                                                                           mSurfaces(), mUIElements(), mTextures(), mShaders(), mCameras(),
                                                                           mScreen(nullptr), mPrimaryCamera(nullptr)
{
  // Add Default Texture
  AddTexturePairing(DEFAULT_TEXTURE_NAME, new TextureData(DEFAULT_TEXTURE_NAME, -1, 0, 0, 
    SystemProperties::GetMinFilter(), SystemProperties::GetMagFilter(), "", "", Vector4(), Vector4(), 0, 0));
#ifdef SHADER_COMPATIBLE
  mScreen = new PCShaderScreen(this, aWidth, aHeight, aFullScreen);
#else
  assert(!"Needs screen for this device.");
#endif
}

GraphicsManager::~GraphicsManager()
{
  for(std::unordered_map<int, TextureData*>::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
  {
    delete it->second;
  }
  for(std::unordered_map<int, ShaderData*>::iterator it = mShaders.begin(); it != mShaders.end(); ++it)
  {
    delete it->second;
  }
  mTextures.clear();
  mShaders.clear();
  ShaderLoader::Clear();
  delete mScreen;
}

/**
 * @brief Update screen, swap buffers, etc.
 */
void GraphicsManager::Update()
{
  std::unordered_map<Camera*, std::vector<Surface*>> cameraObjectRenders = mScreen->PruneObjects(mSurfaces, mCameras);
  mScreen->PreDraw();
  mScreen->SortUI(mUIElements);
  for(std::unordered_map<Camera*, std::vector<Surface*>>::iterator it = cameraObjectRenders.begin(); it != cameraObjectRenders.end(); ++it)
  {
    mScreen->SortObjects(it->second);
    mScreen->Draw(it->second, mUIElements, it->first);
  }
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
}

/**
 * @brief Make this manager visible in LUA.
 */
void GraphicsManager::SerializeLUA()
{
  SLB::Class<GraphicsManager>("GraphicsManager").inherits<Manager>();
}

/**
 * @brief Create a new surface with owner this.
 */
Surface *GraphicsManager::CreateSurface()
{
#ifdef SHADER_COMPATIBLE
  Surface *surface = new PCShaderSurface(this);
#else
  assert(!"Create surface for device.");
#endif

  AddSurface(surface);

  return surface;
}

/**
 * @brief Create surface at UI layer, handy helper.
 */
Surface *GraphicsManager::CreateUISurface()
{
#ifdef SHADER_COMPATIBLE
  Surface *surface = new PCShaderSurface(this);
#else
  assert(!"Create surface for device.");
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
 * @brief Create camera and manage it
 * @return New camera
 */
Camera* GraphicsManager::CreateCamera()
{
  Camera *camera = new Camera(this);
  AddCamera(camera);
  return camera;
}

/**
 * @brief Delete and unmanage camera
 * @param aCamera Camera to delete
 */
void GraphicsManager::DeleteCamera(Camera *aCamera)
{
  RemoveCamera(aCamera);
  delete aCamera;
}

/**
 * @brief Manage camera
 * @param aCamera Camera to manage
 */
void GraphicsManager::AddCamera(Camera *aCamera)
{
  mCameras.insert(aCamera);
  
  if(mPrimaryCamera == nullptr)
  {
    mPrimaryCamera = aCamera;
    mPrimaryCamera->SetPrimary(true);
  }
}

/**
 * @brief Unmanage camera
 * @param aCamera Camera to unmanage
 */
void GraphicsManager::RemoveCamera(Camera *aCamera)
{
  mCameras.erase(aCamera);
  if(aCamera == mPrimaryCamera)
  {
    if(mCameras.size() > 0)
    {
      mPrimaryCamera = *(mCameras.begin());
      mPrimaryCamera->SetPrimary(true);
    }
    else
      mPrimaryCamera = nullptr;
  }
}

/**
 * @brief Remove all cameras
 */
void GraphicsManager::ClearCameras()
{
  for(std::set<Camera*>::iterator it = mCameras.begin(); it != mCameras.end();)
  {
    DeleteCamera(*it);
    it = mCameras.begin();
  }
}

/**
 * @brief Set camera as active viewing camera
 * @param aCamera Camera to set to active
 */
void GraphicsManager::SetPrimaryCamera(Camera *aCamera)
{
  if(aCamera != nullptr && mCameras.find(aCamera) == mCameras.end())
  {
    DebugLogPrint("Attempting to assign primary camera when camera is not being managed.");
    assert(!"Attempting to assign primary camera when camera is not being managed.");
  }
  
  if(mPrimaryCamera)
    mPrimaryCamera->SetPrimary(false);
  
  mPrimaryCamera = aCamera;
  if(mPrimaryCamera)
    mPrimaryCamera->SetPrimary(true);
}

/**
 * @brief Get screen object (the renderer)
 */
Screen *GraphicsManager::GetScreen()
{
  return mScreen;
}

/**
 * @brief Get current viewing camera
 * @return Current viewing camera
 */
Camera *GraphicsManager::GetPrimaryCamera()
{
  return mPrimaryCamera;
}

/**
 * @brief Add texture name and data to map.
 * @param aFilename
 * @param aData
 */
void GraphicsManager::AddTexturePairing(HashString const &aFilename, TextureData *aData)
{
  mTextures.insert(std::pair<int, TextureData*>(aFilename.ToHash(), aData));
}

/**
 * @brief Get data for texture by filename
 * @param aFilename
 */
TextureData* GraphicsManager::GetTextureData(HashString const &aFilename) const
{
  std::unordered_map<int, TextureData*>::const_iterator pos = mTextures.find(aFilename.ToHash());

  if(pos == mTextures.end())
  {
    return mTextures.find(Common::StringHashFunction(DEFAULT_TEXTURE_NAME))->second;
  }

  return pos->second;
}

/**
 * @brief Add shader pairing by name and data.
 * @param aFilename Name of shader file.
 * @param aData Data of shader file.
 */
void GraphicsManager::AddShaderPairing(HashString const &aFilename, ShaderData *aData)
{
  mShaders.insert(std::pair<int, ShaderData*>(aFilename.ToHash(), aData));
}

/**
 * @brief Get data of shader based on file name.
 * @param aFilename Name of shader file.
 * @return Data of shader file. Will assert if file not found.
 */
ShaderData* GraphicsManager::GetShaderData(HashString const &aFilename) const
{
  std::unordered_map<int, ShaderData*>::const_iterator pos = mShaders.find(aFilename.ToHash());

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
  std::unordered_map<int, ShaderData*>::const_iterator pos = mShaders.find(aFilename.ToHash());

  if(pos == mShaders.end())
  {
    return false;
  }

  return true;
}

/**
 * @brief Reset device.
 */
void GraphicsManager::ResetDevice()
{
  ShaderLoader::Clear();
  std::vector<TextureData*> newTextures;
  std::vector<ShaderData*> newShaders;
  std::set<GameObject*> allocatedObjects = GetOwningApp()->GET<ObjectManager>()->GetAllocatedObjects();
  for(std::unordered_map<int, TextureData*>::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
  {
    TextureData* data = it->second;
    TextureData* newTexture = nullptr;
    
    if(!data->mFont.Empty())
      newTexture = ShaderLoader::LoadText(data->mFont, data->mText, data->mMinFilter, data->mMagFilter, 
        data->mForegroundColor, data->mBackgroundColor, data->mSize, data->mMaxWidth);
    else
      newTexture = ShaderLoader::LoadTexture(data->mTextureName, data->mMinFilter, data->mMagFilter);
      
    if(!newTexture)
      newTexture = new TextureData(*data);
    
    newTextures.push_back(newTexture);
    
    for(std::set<GameObject*>::iterator it2 = allocatedObjects.begin(); it2 != allocatedObjects.end(); ++it2)
    {
      mScreen->ResetObjectTexture((*it2)->GET<Surface>(), data, newTexture);
    }
    
    delete data;
  }
  for(std::unordered_map<int, ShaderData*>::iterator it = mShaders.begin(); it != mShaders.end(); ++it)
  {
    ShaderData* data = it->second;
    ShaderData* newShader = ShaderLoader::LoadShaders(data->mVertexFileName, data->mFragmentFileName);
    
    if(!newShader)
      newShader = new ShaderData(*data);
    
    newShaders.push_back(newShader);
    
    for(std::set<GameObject*>::iterator it2 = allocatedObjects.begin(); it2 != allocatedObjects.end(); ++it2)
    {
      mScreen->ResetObjectShader((*it2)->GET<Surface>(), data, newShader);
    }
    
    delete data;
  }
  mTextures.clear();
  mShaders.clear();
  
  for(std::vector<TextureData*>::iterator it = newTextures.begin(); it != newTextures.end(); ++it)
  {
    mTextures.insert(std::pair<int, TextureData*>((*it)->mTextureName.ToHash(), *it));
  }
  for(std::vector<ShaderData*>::iterator it = newShaders.begin(); it != newShaders.end(); ++it)
  {
    HashString shaderKey = (*it)->mVertexFileName + (*it)->mFragmentFileName;
    mShaders.insert(std::pair<int, ShaderData*>(shaderKey.ToHash(), *it));
  }
}

/**
 * @brief Converts absolute coordinate point to point in local screen space.
 * @param aPosition
 */
Vector3 GraphicsManager::AbsToRel(Vector3 const &aPosition) const
{
  Transform *cameraTransform = mPrimaryCamera->GetOwner()->GET<Transform>();
  Vector3 ret = cameraTransform->GetFinalTransform() * cameraTransform->GetPosition();

  ret -= mPrimaryCamera->GetSize() / 2.0f;
  ret += aPosition;
  ret = cameraTransform->GetFinalTransform().Invert() * ret;

  return ret;
}

/**
 * @brief Converts local coordinate point to point in world space.
 * @param aPosition
 */
Vector3 GraphicsManager::RelToAbs(Vector3 const &aPosition) const
{
  Transform *cameraTransform = mPrimaryCamera->GetOwner()->GET<Transform>();
  Vector3 ret = cameraTransform->GetFinalTransform() * aPosition;

  ret += mPrimaryCamera->GetSize() / 2.0f;
  ret -= cameraTransform->GetFinalTransform() * cameraTransform->GetPosition();

  return ret;
}
