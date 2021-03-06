#include "Common.h"
#include "GraphicsManager.h"
#include "ObjectDeleteMessage.h"
#include "ObjectManager.h"
#include "Parsers/ParserFactory.h"

#if !defined(IOS) && !defined(ANDROID)
  #define SHADER_COMPATIBLE
#endif

#ifdef SHADER_COMPATIBLE
  #include "PCShaderScreen.h"
  #include "PCShaderSurface.h"
  #include "ShaderLoader.h"
#endif

#define DEFAULT_TEXTURE_NAME "DefaultEmptyFirstBlank"

int const DEFAULT_QUADTREE_SIZE = 3000;
unsigned const DEFAULT_QUADTREE_CAPACITY = 8;
HashType const GraphicsManager::sUID = Common::StringHashFunction("GraphicsManager");
GraphicsManager::GraphicsManager(GameApp *aApp, int aWidth, int aHeight, bool aFullScreen) : Manager(aApp, "GraphicsManager", GraphicsManager::sUID),
                                                                           mRenderables(), mNewRenderables(), mMovingRenderables(), mUIElements(), mTextures(), mShaders(), mCameras(), 
                                                                           mUnsortedLayers(), mPipelines(), mScreen(nullptr), mPrimaryCamera(nullptr)

{
  // Add Default Texture
  AddTexturePairing(DEFAULT_TEXTURE_NAME, new TextureData(DEFAULT_TEXTURE_NAME, -1, Vector2(), 
    SystemProperties::GetMinFilter(), SystemProperties::GetMagFilter(), "", "", Vector4(), Vector4(), 0, 0));
#ifdef SHADER_COMPATIBLE
  mTree = new QuadTree(DEFAULT_QUADTREE_CAPACITY, Vector3(-DEFAULT_QUADTREE_SIZE, -DEFAULT_QUADTREE_SIZE, 0), Vector3(DEFAULT_QUADTREE_SIZE, DEFAULT_QUADTREE_SIZE, 0));
  mScreen = new PCShaderScreen(this, aWidth, aHeight, aFullScreen);
  CreatePipeline("Default.txt");
#else
  assert(!"Needs screen for this device.");
#endif
}

GraphicsManager::GraphicsManager(GraphicsManager const &aGraphicsManager) : Manager(nullptr, "GraphicsManager", GraphicsManager::sUID),
  mTree(nullptr), mRenderables(), mNewRenderables(), mMovingRenderables(), mUIElements(), mTextures(), mShaders(), mCameras(), 
  mUnsortedLayers(), mPipelines(), mScreen(nullptr), mPrimaryCamera(nullptr)
{
  assert(!"Not allowed");
}

GraphicsManager::~GraphicsManager()
{
  for(std::unordered_map<HashType, TextureData*>::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
  {
    delete it->second;
  }
  for(std::unordered_map<HashType, ShaderData*>::iterator it = mShaders.begin(); it != mShaders.end(); ++it)
  {
    delete it->second;
  }
  for(std::unordered_map<HashType, Pipeline*>::iterator it = mPipelines.begin(); it != mPipelines.end(); ++it)
  {
    delete it->second;
  }
  mTextures.clear();
  mShaders.clear();
  mPipelines.clear();
  ShaderLoader::Clear();
  delete mScreen;
  delete mTree;
}

/**
 * @brief Update screen, swap buffers, etc.
 */
void GraphicsManager::Update()
{
  // Only insert static objects into QuadTree.
  for(RenderableIT it = mNewRenderables.begin(); it != mNewRenderables.end(); ++it)
  {
    GameObject *object = (*it)->GetOwner();
    if(object->GetPlacement() == ObjectPlacement::STATIC)
    {
      mTree->Insert(*it);
    }
    else
    {
      mMovingRenderables.insert(*it);
    }
  }
  mNewRenderables.clear();
  
  std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>> cameraObjectRenders = mScreen->PruneObjects(mTree, mMovingRenderables, mCameras);
  std::vector<Renderable*> uiElements(mUIElements.begin(), mUIElements.end());
  mScreen->PreDraw();
  mScreen->SortUI(uiElements);
  std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>>::iterator cameraObjectRenderEnd = cameraObjectRenders.end();
  for(std::unordered_map<Camera*, std::map<int, std::vector<Renderable*>>>::iterator it = cameraObjectRenders.begin(); it != cameraObjectRenderEnd; ++it)
  {
    std::map<int, std::vector<Renderable*>>::iterator layerEnd = it->second.end();
    for(std::map<int, std::vector<Renderable*>>::iterator it2 = it->second.begin(); it2 != layerEnd; ++it2)
    {
      if(mUnsortedLayers.find(it2->first) == mUnsortedLayers.end())
      {
        mScreen->SortObjects(it2->second);
      }
    }

    // Avoid using layer 999, this line will erase all items in there.
    cameraObjectRenders[it->first][999] = uiElements;
    mScreen->Draw(it->second, it->first);
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
 * @brief Create a new renderable with owner this.
 */
Renderable *GraphicsManager::CreateRenderable(HashString const &aType)
{
#ifdef SHADER_COMPATIBLE
  Renderable *renderable = new PCShaderSurface(this);
#else
  assert(!"Create renderable for device.");
#endif

  AddRenderable(renderable);

  return renderable;
}

/**
 * @brief Create renderable at UI layer, handy helper.
 */
Renderable *GraphicsManager::CreateUIRenderable(HashString const &aType)
{
#ifdef SHADER_COMPATIBLE
  Renderable *renderable = new PCShaderSurface(this);
#else
  assert(!"Create renderable for device.");
#endif

  AddUIRenderable(renderable);

  return renderable;
}

/**
 * @brief Delete and remove renderable.
 * @param aRenderable
 */
void GraphicsManager::DeleteRenderable(Renderable *aRenderable)
{
  RemoveRenderable(aRenderable);
  delete aRenderable;
}

/**
 * @brief Add renderable to list.
 * @param aRenderable
 */
void GraphicsManager::AddRenderable(Renderable *aRenderable)
{
  mRenderables.insert(aRenderable);
  mNewRenderables.insert(aRenderable);
  aRenderable->SetUIElement(false);
}

/**
 * @brief Add renderable to UI list.
 * @param aRenderable
 */
void GraphicsManager::AddUIRenderable(Renderable *aRenderable)
{
  // No need to insert into QuadTree because it is always gonna draw.
  //mNewRenderables.insert(aRenderable);
  mUIElements.insert(aRenderable);
  aRenderable->SetUIElement(true);
}

/**
 * @brief Remove renderable, do not delete.
 * @param aRenderable
 */
void GraphicsManager::RemoveRenderable(Renderable *aRenderable)
{
  mRenderables.erase(aRenderable);
  mNewRenderables.erase(aRenderable);
  mUIElements.erase(aRenderable);
  mMovingRenderables.erase(aRenderable);
  mTree->Remove(aRenderable);
}

/**
 * @brief Remove and delete all renderables
 */
void GraphicsManager::ClearRenderables()
{
  for(RenderableIT it = mRenderables.begin(); it != mRenderables.end();)
  {
    DeleteRenderable(*it);
    mTree->Remove(*it);
    it = mRenderables.begin();
  }
  for(RenderableIT it = mUIElements.begin(); it != mUIElements.end();)
  {
    DeleteRenderable(*it);
    it = mUIElements.begin();
  }
  mMovingRenderables.clear();
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
  for(std::unordered_set<Camera*>::iterator it = mCameras.begin(); it != mCameras.end();)
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
 * @brief Create pipeline from file
 * @param aFileName Name of file, located in Shaders folder
 * @return New pipeline
 */
Pipeline* GraphicsManager::CreatePipeline(HashString const &aFileName)
{
  Parser *parser = ParserFactory::CreateInputParser("Shaders", aFileName);
  HashString const STEP = "Step_";
  HashString const PASS = "Pass_";
  
  ParserNode *pipelineNode = parser->GetBaseRoot();
  HashString name = pipelineNode->Find("Name")->GetValue();
  
  Pipeline *pipeline = new Pipeline(name);
  int stepIndex = 0;
  HashString currentStep = STEP + Common::IntToString(stepIndex);
  while(pipelineNode->Find(currentStep))
  {
    ParserNode *stepNode = pipelineNode->Find(currentStep);
    int passIndex = 0;
    HashString currentPass = PASS + Common::IntToString(passIndex);
    while(stepNode->Find(currentPass))
    {
      ParserNode *passNode = stepNode->Find(currentPass);
      HashString vertexShader = passNode->Find("VertexShader")->GetValue();
      HashString fragmentShader = passNode->Find("FragmentShader")->GetValue();
      Vector4 clearColor = Vector4(0,0,0,1);
      if(passNode->Find("ClearColor"))
      {
        clearColor = passNode->Find("ClearColor")->GetValue().ToVector4();
      }
      
      Pass *pass = new Pass(this, vertexShader, fragmentShader, clearColor);
      pipeline->AddPass(stepIndex, pass);
      
      ++passIndex;
      currentPass = PASS + Common::IntToString(passIndex);
    }
    
    ++stepIndex;
    currentStep = STEP + Common::IntToString(stepIndex);
  }
  
  mPipelines[name.ToHash()] = pipeline;
  return pipeline;
}

/**
 * @brief Get pipeline
 * @param aId Id of pipeline
 * @return Pipeline if available, otherwise nullptr
 */
Pipeline* GraphicsManager::GetPipeline(HashType const &aId)
{
  if(mPipelines.find(aId) == mPipelines.end())
    return nullptr;
    
  return mPipelines[aId];
}

/**
 * @brief Set unsorted layers, optimization
 * @param aUnsortedLayers
 */
void GraphicsManager::SetUnsortedLayers(std::unordered_set<int> const aUnsortedLayers)
{
  mUnsortedLayers = aUnsortedLayers;
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
 * @brief Get Tree
 * @return Tree
 */
Tree* GraphicsManager::GetTree()
{
  return mTree;
}

/**
 * @brief Add texture name and data to map.
 * @param aKey
 * @param aData
 */
void GraphicsManager::AddTexturePairing(HashString const &aKey, TextureData *aData)
{
  mTextures.insert(std::pair<HashType, TextureData*>(aKey.ToHash(), aData));
}

/**
 * @brief Get data for texture by filename
 * @param aKey
 */
TextureData* GraphicsManager::GetTextureData(HashString const &aKey) const
{
  std::unordered_map<HashType, TextureData*>::const_iterator pos = mTextures.find(aKey.ToHash());

  if(pos == mTextures.end())
  {
    HashType hash = Common::StringHashFunction(DEFAULT_TEXTURE_NAME);
    return mTextures.find(hash)->second;
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
  mShaders.insert(std::pair<HashType, ShaderData*>(aFilename.ToHash(), aData));
}

/**
 * @brief Get data of shader based on file name.
 * @param aFilename Name of shader file.
 * @return Data of shader file. Will assert if file not found.
 */
ShaderData* GraphicsManager::GetShaderData(HashString const &aFilename) const
{
  std::unordered_map<HashType, ShaderData*>::const_iterator pos = mShaders.find(aFilename.ToHash());

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
  std::unordered_map<HashType, ShaderData*>::const_iterator pos = mShaders.find(aFilename.ToHash());

  if(pos == mShaders.end())
  {
    return false;
  }

  return true;
}

/**
 * @brief Set shader property globally for program. HINT: If you're not seeing your intended results, remember
 *        to declare the field as uniform in your shader.
 * @param aVertexShaderFilename Vertex shader file name.
 * @param aFragmentShaderFilename Fragment shader file name.
 * @param aProperty Property to set shader to.
 */
void GraphicsManager::SetGlobalShaderProperty(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename,
                                            RenderableProperty const &aProperty)
{
  HashString key = aVertexShaderFilename + aFragmentShaderFilename;
  if(ShaderDataExists(key))
  {
    mScreen->SetGlobalShaderProperty(GetShaderData(key), aProperty);
  }
}

/**
 * @brief Reset device.
 */
void GraphicsManager::ResetDevice()
{
  ShaderLoader::Clear();
  std::vector<TextureData*> newTextures;
  std::vector<ShaderData*> newShaders;
  std::unordered_set<GameObject*> allocatedObjects = GetOwningApp()->GET<ObjectManager>()->GetAllocatedObjects();
  for(std::unordered_map<HashType, TextureData*>::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
  {
    TextureData* data = it->second;
    TextureData* newTexture = nullptr;
    
    if(!data->mFont.Empty())
      newTexture = ShaderLoader::LoadText(data->mFont, data->mText, data->mMinFilter, data->mMagFilter, 
        data->mForegroundColor, data->mBackgroundColor, data->mFontSize, data->mMaxWidth);
    else
      newTexture = ShaderLoader::LoadTexture(data->mTextureName, data->mMinFilter, data->mMagFilter);
      
    if(!newTexture)
      newTexture = new TextureData(*data);
    
    newTextures.push_back(newTexture);
    
    for(std::unordered_set<GameObject*>::iterator it2 = allocatedObjects.begin(); it2 != allocatedObjects.end(); ++it2)
    {
      mScreen->ResetObjectTexture((*it2)->GET<Renderable>(), data, newTexture);
    }
    
    delete data;
  }
  for(std::unordered_map<HashType, ShaderData*>::iterator it = mShaders.begin(); it != mShaders.end(); ++it)
  {
    ShaderData* data = it->second;
    ShaderData* newShader = ShaderLoader::LoadShaders(data->mVertexFileName, data->mFragmentFileName);
    
    if(!newShader)
      newShader = new ShaderData(*data);
    
    newShaders.push_back(newShader);
    
    for(std::unordered_set<GameObject*>::iterator it2 = allocatedObjects.begin(); it2 != allocatedObjects.end(); ++it2)
    {
      mScreen->ResetObjectShader((*it2)->GET<Renderable>(), data, newShader);
    }
    
    delete data;
  }
  mTextures.clear();
  mShaders.clear();
  
  for(std::vector<TextureData*>::iterator it = newTextures.begin(); it != newTextures.end(); ++it)
  {
    mTextures.insert(std::pair<HashType, TextureData*>((*it)->mTextureName.ToHash(), *it));
  }
  for(std::vector<ShaderData*>::iterator it = newShaders.begin(); it != newShaders.end(); ++it)
  {
    HashString shaderKey = (*it)->mVertexFileName + (*it)->mFragmentFileName;
    mShaders.insert(std::pair<HashType, ShaderData*>(shaderKey.ToHash(), *it));
  }
}

/**
 * @brief Resize tree range for optimization purposes.
 * @param aMinRange Minimum range.
 * @param aMaxRange Maximum range.
 */
void GraphicsManager::ResizeTree(Vector3 const &aMinRange, Vector3 const &aMaxRange)
{
  mTree->Resize(aMinRange, aMaxRange);
}

/**
 * @brief Clear tree.
 */
void GraphicsManager::ClearTree()
{
  mTree->Clear();
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
