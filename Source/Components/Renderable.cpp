#include "Renderable.h"
#include "LUATypes.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "Transform.h"

#define DT (1.0f/60.0f)

int const Renderable::sUID = Common::StringHashFunction("Renderable");

Renderable::Renderable() : Component(Renderable::sUID), mTexCoord(nullptr), mViewmode(VIEW_ABSOLUTE),
                     mTextureSize(), mPrimaryColor(1,1,1,1), mNoRender(false), 
                     mMinFilter(SystemProperties::GetMinFilter()), mMagFilter(SystemProperties::GetMagFilter()), 
                     mLayer(0), mIsolatedRenderLayers(), mFileName(), mText(), mFontName(), mFontSize(0), mMaxTextWidth(0), mSecondaryColor(),
                     mOriginalSize(), mTextRenderStyle(DEFAULT_RENDER_STYLE), mScrollInfo(), mProperties(), mUIElement(false)
{
  assert(!"Renderable needs a graphicsmanager");
}

Renderable::Renderable(Renderable const &aRenderable) : Component(Renderable::sUID), mTexCoord(new TextureCoordinates(*(aRenderable.mTexCoord))), mManager(aRenderable.mManager),
                                            mViewmode(aRenderable.mViewmode), mTextureSize(aRenderable.mTextureSize),
                                            mPrimaryColor(aRenderable.mPrimaryColor), mNoRender(aRenderable.mNoRender),
                                            mMinFilter(aRenderable.mMinFilter), mMagFilter(aRenderable.mMagFilter), mLayer(aRenderable.mLayer),
                                            mIsolatedRenderLayers(aRenderable.mIsolatedRenderLayers),
                                            mFileName(aRenderable.mFileName), mText(aRenderable.mText), mFontName(aRenderable.mFontName),
                                            mFontSize(aRenderable.mFontSize), mMaxTextWidth(aRenderable.mMaxTextWidth),
                                            mSecondaryColor(aRenderable.mSecondaryColor), mOriginalSize(aRenderable.mOriginalSize),
                                            mTextRenderStyle(aRenderable.mTextRenderStyle), mScrollInfo(aRenderable.mScrollInfo), mUIElement(false)
{
  PropertyContainerConstIt propertyEnd = aRenderable.mProperties.end();
  for(PropertyContainerConstIt it = aRenderable.mProperties.begin(); it != propertyEnd; ++it)
  {
    mProperties[it->second->GetName().ToHash()] = new RenderableProperty(*(it->second));
  }
}

Renderable::Renderable(GraphicsManager *aManager) : Component(Renderable::sUID), mTexCoord(nullptr),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mTextureSize(), mPrimaryColor(1,1,1,1), mNoRender(false), 
                                              mMinFilter(SystemProperties::GetMinFilter()), mMagFilter(SystemProperties::GetMagFilter()),
                                              mLayer(0), mIsolatedRenderLayers(), mFileName(), mText(), mFontName(), mFontSize(0), 
                                              mMaxTextWidth(0), mSecondaryColor(), mOriginalSize(),
                                              mTextRenderStyle(DEFAULT_RENDER_STYLE), mScrollInfo(), mProperties(), mUIElement(false)
{
}

Renderable::~Renderable()
{
  ClearProperties();
  mManager = nullptr;
  if(mTexCoord)
  {
    delete mTexCoord;
    mTexCoord = nullptr;
  }
}

/**
 * @brief Asserts, please implement the platform specific implementation.
 * @param aName Nothing.
 */
void Renderable::LoadImage(HashString const &aName)
{
  assert(!"Not supported (Renderable LoadImage)");
}

/**
 * @brief Asserts, please implement the platform specific implementation.
 * @param aText Nothing.
 * @return Nothing.
 */
void Renderable::LoadText(HashString const &aText)
{
  assert(!"Not supported (Renderable LoadText)");
}

/**
 * @brief Asserts, please implement the platform specific implementation.
 * @param aVertexShaderFilename Nothing.
 * @param aFragmentShaderFilename Nothing.
 */
void Renderable::LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  assert(!"Not supported (Renderable LoadShaders)");
}

/**
 * @brief Get texture coordinate data (animation data)
 * @return Texture coordinate data.
 */
TextureCoordinates *Renderable::GetTextureData() const
{
  return mTexCoord;
}

/**
 * @brief Set up animation data helper
 * @param aNumAnimations Total number of animations
 * @param aNumFrames Number of frames per animation
 * @param aAnimationSpeed Time between frames
 */
void Renderable::SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const &aNumFrames, 
                                       std::vector<std::vector<float>> const &aAnimationSpeeds)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(mTextureSize.x, mTextureSize.y, aNumAnimations, aNumFrames, aAnimationSpeeds);
}

/**
 * @brief Set up animation data helper
 * @param aNumColumns Columns in file
 * @param aNumRows Rows in file
 * @param aAnimations Animation data
 */
void Renderable::SetTextureCoordinateData(int const aNumColumns, int const aNumRows, std::vector<AnimationInfo*> const &aAnimations)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(mTextureSize.x, mTextureSize.y, aNumColumns, aNumRows, aAnimations);
}

/**
 * @brief Set whether this surface is animated
 * @param aAnimated True is animated, false otherwise
 */
void Renderable::SetAnimated(bool aAnimated)
{
  if(mTexCoord)
    mTexCoord->SetAnimated(aAnimated);
}

/**
 * @brief Get current animation
 * @param aAnimation Animation id to run
 * @param aBehavior Set behavior of animation (run once and stop or run and reset, etc.)
 */
void Renderable::SetAnimation(int aAnimation, int aBehavior)
{
  if(mTexCoord)
  {
    mTexCoord->SetCurrentAnimation(aAnimation, (TextureCoordinateBehavior)aBehavior);
    mTexCoord->SetBehavior((TextureCoordinateBehavior)aBehavior);
  }
}

/**
 * @brief Set current frame of animation
 * @param aFrame Frame of current animation
 */
void Renderable::SetCurrentFrame(int aFrame)
{
  if(mTexCoord)
    mTexCoord->SetCurrentFrame(aFrame);
}

/**
 * @brief Set frame by id in animation file. (Regardless of animation)
 * @param aFrameID The id to set to.
 */
void Renderable::SetFrameByID(int aFrameID)
{
  if(mTexCoord)
    mTexCoord->SetFrameByID(aFrameID);
}

/**
 * @brief Set speed of current animation
 * @param aAnimationSpeed Time between frames (in seconds)
 */
void Renderable::SetAnimationSpeed(float aAnimationSpeed)
{
  if(mTexCoord)
    mTexCoord->SetCurrentAnimationSpeed(aAnimationSpeed);
}

/**
 * @brief Force finish animation
 */
void Renderable::FinishAnimation()
{
  if(mTexCoord)
    mTexCoord->Finish();
}

/**
 * @brief Check if current animation has iterated at least once
 * @return True if true.
 */
bool Renderable::CurrentAnimationCompleted()
{
  if(mTexCoord)
    return mTexCoord->GetCompleted();
  return false;
}

bool Renderable::RenderablePropertiesEquals(Renderable const *aRenderable) const
{
  PropertyContainer const& otherRenderableProperties = ((Renderable*)aRenderable)->mProperties;
  if(mProperties.size() != otherRenderableProperties.size())
  {
    return false;
  }

  PropertyContainerConstIt end = mProperties.end();
  for(PropertyContainerConstIt it = mProperties.begin(); it != mProperties.end(); ++it) 
  {
    RenderableProperty *property = it->second;
    int hash = property->GetName().ToHash();
    PropertyContainerConstIt otherPropertyIt = otherRenderableProperties.find(hash);
    if(otherPropertyIt == otherRenderableProperties.end())
    {
      return false;
    }
    else if(*otherPropertyIt->second != *property)
    {
      return false;
    }
  }
  return true;
}

/**
 * @brief Create scrolling reveal effect.
 * @param aScrollType HORIZONTAL or VERTICAL.
 * @param aGoalSize Goal size of the object (in world space).
 */
void Renderable::CreateScrollEffect(ScrollType const& aScrollType, Vector3 const& aGoalSize)
{
  ScrollInfo scrollInfo;
  scrollInfo.mType = aScrollType;
  scrollInfo.mGoalSize = Vector2(aGoalSize.x, aGoalSize.y);
  mScrollInfo.push_back(scrollInfo);
}

/**
 * @brief Get properties container
 * @return Container of properties
 */
PropertyContainer const& Renderable::GetProperties() const
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
void Renderable::AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue)
{
  if(mProperties.find(aName.ToHash()) != mProperties.end())
  {
    RenderableProperty *surfaceProperty = mProperties[aName.ToHash()];
    surfaceProperty->SetType(aType);
    surfaceProperty->SetTargetValue(aTargetValue);
    surfaceProperty->SetDefaultValue(aDefaultValue);
    return;
  }
  mProperties[aName.ToHash()] = new RenderableProperty(aName, aType, aTargetValue, aDefaultValue);
}

/**
 * @brief Edit property if there, otherwise add.
 * @param aName Name of property
 * @param aType Type of property
 * @param aTargetValue Target value of property
 * @param aDefaultValue Default value of property
 * @param aId Id of property
 */
void Renderable::AddOrEditPropertyWithId(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue, HashString const &aId)
{
  if(mProperties.find(aName.ToHash()) != mProperties.end())
  {
    RenderableProperty *surfaceProperty = mProperties[aName.ToHash()];
    surfaceProperty->SetType(aType);
    surfaceProperty->SetTargetValue(aTargetValue);
    surfaceProperty->SetDefaultValue(aDefaultValue);
    surfaceProperty->SetId(aId);
    return;
  }
  mProperties[aName.ToHash()] = new RenderableProperty(aName, aType, aTargetValue, aDefaultValue, aId);
}

/**
 * @brief Clear properties for a surface.
 */
void Renderable::ClearProperties()
{
  PropertyContainerIt propertyEnd = mProperties.end();
  for(PropertyContainerIt it = mProperties.begin(); it != propertyEnd; ++it)
  {
    delete it->second;
  }
  mProperties.clear();
}

/**
 * @brief Simple update loop.
 */
void Renderable::Update()
{
  float dt = GetOwner()->GetManager()->GetOwningApp()->GetAppStep();

  if(mTexCoord)
    mTexCoord->Update(dt);
    
  if(!mScrollInfo.empty())
  {
    Transform *transform = GetOwner()->GET<Transform>();
    
    ScrollInfoIT end = mScrollInfo.end();
    for(ScrollInfoIT it = mScrollInfo.begin(); it != end; ++it)
    {
      if(it->mType == HORIZONTAL)
      {
        transform->GetSize().x = GetTextureData()->GetXValue(1) * it->mGoalSize.x;
      }
      else if(it->mType == VERTICAL)
      {
        transform->GetSize().y = GetTextureData()->GetYValue(1) * it->mGoalSize.y;
      }
      else
      {
        assert(!"Invalid type for scrolling.");
      }
    }
  }
}

/**
 * @brief Send message to owner for other components
 * @param aMessage Message to send.
 */
void Renderable::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive a message
 * @param aMessage Received message
 */
void Renderable::ReceiveMessage(Message const &aMessage)
{
  if(aMessage.GetDescription() == "Finish")
  {
    if(!mText.Empty())
    {
      Transform *transform = GetOwner()->GET<Transform>();
      transform->SetSize(GetOriginalSize());
    }
    FinishAnimation();
  }
}

/**
 * @brief Serialize to file
 * @param aNode ParserNode to write to.
 */
void Renderable::Serialize(ParserNode *aNode)
{
  HashString const SURFACE = "Renderable";
  TextureCoordinates *coords = GetTextureData();
  std::vector<int> animations;
  bool animated = coords->GetAnimated();
  int numanimations = coords->GetNumberOfAnimations();
  std::vector<std::vector<float>> animationSpeeds;
  int currentAnimation = coords->GetCurrentAnimation();
  char const* values[4] = {"ColorR",
                           "ColorG",
                           "ColorB",
                           "ColorA"};

  aNode->Place(SURFACE, "");
  ParserNode* surface = aNode->Find(SURFACE);
  
  // Animations
  surface->Place("Animations", "");
  HashString const ANIMATION = "Animation_";
  int curIndex = 0;
  ParserNode* animationsNode = surface->Find("Animations");
  animationsNode->Place("Columns", Common::IntToString(mTexCoord->GetMaxFrames()));
  animationsNode->Place("Rows", Common::IntToString(mTexCoord->GetTotalFrames() / mTexCoord->GetMaxFrames()));
  
  for(std::vector<AnimationInfo*>::const_iterator it = mTexCoord->GetAllAnimations().begin(); it != mTexCoord->GetAllAnimations().end(); ++it, ++curIndex)
  {
    AnimationInfo const *info = *it;
    HashString curNode = ANIMATION + Common::IntToString(curIndex);
    animationsNode->Place(curNode, "");
    ParserNode* animationInfosNode = animationsNode->Find(curNode);
    
    animationInfosNode->Place("Speed", Common::FloatVectorToString(info->mSpeeds));
    animationInfosNode->Place("SpeedModifier", Common::FloatToString(info->mSpeedModifier));
    animationInfosNode->Place("Frames", Common::IntVectorToString(info->mFrames));
    animationInfosNode->Place("LoopStartFrame", Common::IntToString(info->mLoopStartFrame));
  }
  
  // Everything else animation related
  surface->Place("Animated", Common::BoolToString(animated));
  surface->Place("NoRender", Common::BoolToString(mNoRender));
  surface->Place("StartingAnimation", Common::IntToString(currentAnimation));
  surface->Place("XBias", Common::DoubleToString(mTexCoord->GetBias(0)));
  surface->Place("YBias", Common::DoubleToString(mTexCoord->GetBias(1)));
  for(int i = 0; i < 4; ++i)
  {
    surface->Place(values[i], Common::IntToString(mPrimaryColor[i]));
  }
  
  surface->Place("MinFilter", mMinFilter);
  surface->Place("MagFilter", mMagFilter);
  surface->Place("Layer", Common::IntToString(mLayer));
  if(!mIsolatedRenderLayers.empty())
  {
    surface->Place("IsolatedLayers", Common::IntVectorToString(mIsolatedRenderLayers));
  }
  
  // View mode
  HashString viewMode = "ABSOLUTE";
  if(mViewmode == VIEW_RELATIVE_TO_CAMERA)
    viewMode = "RELATIVE";
  else if(mViewmode == VIEW_PERCENTAGE_OF_CAMERA)
    viewMode = "PERCENTAGE";
  surface->Place("ViewMode", viewMode.ToString());
  
  // Properties
  if(!mProperties.empty())
  {
    surface->Place("Properties", "");
    HashString const PROPERTY = "Property_";
    curIndex = 0;
    ParserNode* propertiesNode = surface->Find("Properties");
    for(PropertyContainerIt it = mProperties.begin(); it != mProperties.end(); ++it, ++curIndex)
    {
      HashString curNode = PROPERTY + Common::IntToString(curIndex);
      propertiesNode->Place(curNode, "");
      
      HashString type = "";
      RenderableProperty* surfaceProperty = it->second;
      switch(surfaceProperty->GetType())
      {
      case PropertyType::INT1:
        type = "INT1";
        break;
      case PropertyType::INT2:
        type = "INT2";
        break;
      case PropertyType::INT3:
        type = "INT3";
        break;
      case PropertyType::INT4:
        type = "INT4";
        break;
      case PropertyType::FLOAT1:
        type = "FLOAT1";
        break;
      case PropertyType::FLOAT2:
        type = "FLOAT2";
        break;
      case PropertyType::FLOAT3:
        type = "FLOAT3";
        break;
      case PropertyType::FLOAT4:
        type = "FLOAT4";
        break;
      case PropertyType::SAMPLER2:
        type = "SAMPLER2";
        break;
      default:
        assert(!"Invalid PropertyType trying to be serialized.");
        break;
      }
      
      ParserNode* propertyNode = propertiesNode->Find(curNode);
      propertyNode->Place("Name", surfaceProperty->GetName());
      propertyNode->Place("Type", type);
      propertyNode->Place("TargetValue", surfaceProperty->GetTargetValue());
      propertyNode->Place("DefaultValue", surfaceProperty->GetDefaultValue());
    }
  }
}

/**
 * @brief Read from file
 * @param aNode ParserNode to read from.
 */
void Renderable::Deserialize(ParserNode *aNode)
{
  float const frameRate = GetManager()->GetOwningApp()->GetAppStep();
  bool animated = false;
  bool frameBased = false;
  int numAnimations = 1;
  int startingAnimation = 0;
  double xBias = 0;
  double yBias = 0;
  std::vector<std::vector<float>> animationSpeed;
  std::vector<int> numFrames;
  numFrames.push_back(1);
  
  // Autofill values if already in place.
  if(mTexCoord)
  {
    numFrames.clear();
    animated = mTexCoord->GetAnimated();
    numAnimations = mTexCoord->GetNumberOfAnimations();
    animationSpeed = mTexCoord->GetAllAnimationHolds();
    numFrames = mTexCoord->GetAllAnimationFrameCounts();
  }
  
  // File sifting.
  if(aNode->Find("FrameBased"))
  {
    frameBased = aNode->Find("FrameBased")->GetValue().ToBool();
  }
  
  // Animation
  if(aNode->Find("Animations"))
  {
    HashString const nodeName = "Animation_";
    int index = 0;
    HashString curIndex = nodeName + Common::IntToString(index);
    ParserNode *animationNode = aNode->Find("Animations");
    std::vector<AnimationInfo*> animationInfos;
    
    int numRows = animationNode->Find("Rows")->GetValue().ToInt();
    int numColumns = animationNode->Find("Columns")->GetValue().ToInt();
    bool isAnimated = aNode->Find("Animated")->GetValue().ToBool();
    int loopFrame = 0;
    if(isAnimated)
      animated = true;
    while(animationNode->Find(curIndex))
    {
      ParserNode *currentAnimation = animationNode->Find(curIndex);
      float speedModifier = 1;
      if(currentAnimation->Find("SpeedModifier"))
      {
        speedModifier = currentAnimation->Find("SpeedModifier")->GetValue().ToFloat();
      }
      
      std::vector<int> frames;
      if(currentAnimation->Find("Frames"))
      {
        frames = currentAnimation->Find("Frames")->GetValue().ToIntVector();
      }
      else if(currentAnimation->Find("NumFrames"))
      {
        int numFrames = currentAnimation->Find("NumFrames")->GetValue().ToInt();
        int rowOffset = index;
        int columnOffset = 0;
        if(currentAnimation->Find("RowOffset"))
          rowOffset = currentAnimation->Find("RowOffset")->GetValue().ToInt();
        if(currentAnimation->Find("ColumnOffset"))
          columnOffset = currentAnimation->Find("ColumnOffset")->GetValue().ToInt();
        
        for(int i = 0; i < numFrames; ++i)
        {
          frames.push_back(i + columnOffset + (numColumns * rowOffset));
        }
      }
      else
      {
        assert(!"Animation passed in without frames");
      }
      
      if(animationNode->Find("LoopStartFrame"))
      {
        loopFrame = animationNode->Find("LoopStartFrame")->GetValue().ToInt();
      }
      
      std::vector<float> speeds;
      if(currentAnimation->Find("Speeds"))
      {
        speeds = currentAnimation->Find("Speeds")->GetValue().ToFloatVector();
      }
      else if(currentAnimation->Find("Speed"))
      {
        float speed = currentAnimation->Find("Speed")->GetValue().ToFloat();
        for(int i = 0; i < frames.size(); ++i)
        {
          speeds.push_back(speed);
        }
      }
      else
      {
        for(int i = 0; i < frames.size(); ++i)
        {
          speeds.push_back(frameRate);
        }
      }
      
      AnimationInfo *info = new AnimationInfo(index, speedModifier, speeds, frames, loopFrame);
      animationInfos.push_back(info);
      
      ++index;
      curIndex = nodeName + Common::IntToString(index);
    }
    
    SetTextureCoordinateData(numColumns, numRows, animationInfos);
  }
  else
  {
    if(aNode->Find("AnimationCount"))
    {
      ParserNode* animationCount = aNode->Find("AnimationCount");

      numFrames.clear();
      numAnimations = animationCount->GetValue().ToInt();
      numFrames = aNode->Find("FrameNumbers")->GetValue().ToIntVector();
      
      bool isAnimated = aNode->Find("Animated")->GetValue().ToBool();
      if(isAnimated)
        animated = true;
    }
    // Supports compatibility mode with old and really old files, support single animation speed or multiple.
    if(aNode->Find("AnimationSpeeds") && aNode->Find("AnimationSpeeds")->Find("Animation_0"))
    {
      // Clear animation speed.
      animationSpeed.clear();
      
      // Optional parameter to change the animation speeds.
      HashString const nodeName = "Animation_";
      int index = 0;
      HashString curIndex = nodeName + Common::IntToString(index);
      ParserNode* animationSpeedNode = aNode->Find("AnimationSpeeds");
      
      while(animationSpeedNode->Find(curIndex))
      {
        ParserNode* curNode = animationSpeedNode->Find(curIndex);
        std::vector<float> singleSpeeds = curNode->GetValue().ToFloatVector();
        
        // Convert from frames to time.
        if(frameBased)
        {
          for(unsigned i = 0; i < singleSpeeds.size(); ++i)
          {
            singleSpeeds[i] = singleSpeeds[i] * frameRate;
          }
        }
        
        animationSpeed.push_back(singleSpeeds);
        ++index;
        curIndex = nodeName + Common::IntToString(index);
      }
      SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
    }
    else if(aNode->Find("AnimationSpeeds"))
    {
      // Clear animation speed.
      animationSpeed.clear();
      
      // Optional parameter to change the animation speeds.
      ParserNode* animationSpeedNode = aNode->Find("AnimationSpeeds");
      std::vector<float> singleSpeeds = animationSpeedNode->GetValue().ToFloatVector();
      
      for(unsigned i = 0; i < singleSpeeds.size(); ++i)
      {
        std::vector<float> speedVector;
        for(int j = 0; j < numFrames[i]; ++j)
        {
          speedVector.push_back((frameBased) ? singleSpeeds[i] * frameRate : singleSpeeds[i]);
        }
        animationSpeed.push_back(speedVector);
      }
      SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
    }
    else if(aNode->Find("AnimationSpeed"))
    {
      // Clear animation speed.
      animationSpeed.clear();
      
      // Optional parameter to change the animation speed, using a single number.
      ParserNode* animationSpeedNode = aNode->Find("AnimationSpeed");
      for(int i = 0; i < numAnimations; ++i)
      {
        animationSpeed.push_back(std::vector<float>());
        for(int j = 0; j < numFrames[i]; ++j)
        {
          float speed = animationSpeedNode->GetValue().ToFloat();
          animationSpeed[i].push_back((frameBased) ? speed * frameRate : speed);
        }
      }
      SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
    }
    else if(animationSpeed.empty())
    {
      // Default to DT
      for(int i = 0; i < numAnimations; ++i)
      {
        animationSpeed.push_back(std::vector<float>());
        for(int j = 0; j < numFrames[i]; ++j)
        {
          animationSpeed[i].push_back(frameRate);
        }
      }
      SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
    }
    
    // Used to be here, but removed since we may not need to set texture data.
    // SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
  }
  
  // Rendering, color, etc.
  if(aNode->Find("NoRender"))
  {
    mNoRender = aNode->Find("NoRender")->GetValue().ToBool();
  }
  if(aNode->Find("ColorR"))
  {
    float red = aNode->Find("ColorR")->GetValue().ToFloat();
    float green = aNode->Find("ColorG")->GetValue().ToFloat();
    float blue = aNode->Find("ColorB")->GetValue().ToFloat();
    float alpha = aNode->Find("ColorA")->GetValue().ToFloat();

    mPrimaryColor = Vector4(red, green, blue, alpha);
  }
  if(aNode->Find("PrimaryColor"))
  {
    ParserNode* primaryColor = aNode->Find("PrimaryColor");
    mPrimaryColor.x = primaryColor->Find("r")->GetValue().ToFloat();
    mPrimaryColor.y = primaryColor->Find("g")->GetValue().ToFloat();
    mPrimaryColor.z = primaryColor->Find("b")->GetValue().ToFloat();
    mPrimaryColor.w = primaryColor->Find("a")->GetValue().ToFloat();
  }
  if(aNode->Find("SecondaryColor"))
  {
    ParserNode* secondaryColor = aNode->Find("SecondaryColor");
    mSecondaryColor.x = secondaryColor->Find("r")->GetValue().ToFloat();
    mSecondaryColor.y = secondaryColor->Find("g")->GetValue().ToFloat();
    mSecondaryColor.z = secondaryColor->Find("b")->GetValue().ToFloat();
    mSecondaryColor.w = secondaryColor->Find("a")->GetValue().ToFloat();
  }
  if(aNode->Find("ViewMode"))
  {
    HashString viewMode = aNode->Find("ViewMode")->GetValue();
    if(viewMode == "ABSOLUTE")
      mViewmode = VIEW_ABSOLUTE;
    else if(viewMode == "RELATIVE")
      mViewmode = VIEW_RELATIVE_TO_CAMERA;
    else if(viewMode == "PERCENTAGE")
      mViewmode = VIEW_PERCENTAGE_OF_CAMERA;
    else
      assert(!"Invalid value passed into ViewMode for Renderable. (Renderable.cpp)");  
  }
  if(aNode->Find("StartingAnimation"))
  {
    startingAnimation = aNode->Find("StartingAnimation")->GetValue().ToInt();
  }
  if(aNode->Find("Properties"))
  {
    HashString const nodeName = "Property_";
    int index = 0;
    HashString curIndex = nodeName + Common::IntToString(index);
    ParserNode* propertyNode = aNode->Find("Properties");
    
    while(propertyNode->Find(curIndex))
    {
      ParserNode* curNode = propertyNode->Find(curIndex);
      PropertyType propertyType = PropertyType::INT1;
      HashString const type = curNode->Find("Type")->GetValue();
      
      if(type == "INT1")
        propertyType = PropertyType::INT1;
      else if(type == "INT2")
        propertyType = PropertyType::INT2;
      else if(type == "INT3")
        propertyType = PropertyType::INT3;
      else if(type == "INT4")
        propertyType = PropertyType::INT4;
      else if(type == "FLOAT1")
        propertyType = PropertyType::FLOAT1;
      else if(type == "FLOAT2")
        propertyType = PropertyType::FLOAT2;
      else if(type == "FLOAT3")
        propertyType = PropertyType::FLOAT3;
      else if(type == "FLOAT4")
        propertyType = PropertyType::FLOAT4;
      else if(type == "SAMPLER2")
        propertyType = PropertyType::SAMPLER2;
      else
      {
        DebugLogPrint("Invalid value %s passed into Property deserialization.", type.ToCharArray());
        assert(!"Invalid value passed into Property deserialization.");
      }
      
      AddOrEditProperty(curNode->Find("Name")->GetValue(), propertyType, 
        curNode->Find("TargetValue")->GetValue(), curNode->Find("DefaultValue")->GetValue());
      
      ++index;
      curIndex = nodeName + Common::IntToString(index);
    }
  }
  if(aNode->Find("XBias"))
  {
    xBias = aNode->Find("XBias")->GetValue().ToDouble();
  }
  if(aNode->Find("YBias"))
  {
    yBias = aNode->Find("YBias")->GetValue().ToDouble();
  }
  if(aNode->Find("MinFilter"))
  {
    mMinFilter = aNode->Find("MinFilter")->GetValue();
  }
  if(aNode->Find("MagFilter"))
  {
    mMagFilter = aNode->Find("MagFilter")->GetValue();
  }
  if(aNode->Find("Layer"))
  {
    mLayer = aNode->Find("Layer")->GetValue().ToInt();
  }
  if(aNode->Find("IsolatedLayers"))
  {
    mIsolatedRenderLayers = aNode->Find("IsolatedLayers")->GetValue().ToIntVector();
  }
  
  // Text
  mMaxTextWidth = mManager->GetScreen()->GetWidth();
  if(aNode->Find("Font"))
  {
    mFontName = aNode->Find("Font")->Find("Name")->GetValue().ToString();
    mFontSize = Common::StringToInt(aNode->Find("Font")->Find("Size")->GetValue());
  }
  if(aNode->Find("MaxTextWidth"))
  {
    mMaxTextWidth = aNode->Find("MaxTextWidth")->GetValue().ToInt();
  }
  if(aNode->Find("Contents"))
  {
    mText = aNode->Find("Contents")->GetValue().ToString();
  }
  if(aNode->Find("RenderStyle"))
  {
    if(aNode->Find("RenderStyle")->GetValue() == "SMOOTH")
      mTextRenderStyle = TextRenderStyle::SMOOTH_RENDER_STYLE;
    else if(aNode->Find("RenderStyle")->GetValue() == "CHARACTER_BY_CHARACTER")
      mTextRenderStyle = TextRenderStyle::CHARACTER_BY_CHARACTER_STYLE;
    else if(aNode->Find("RenderStyle")->GetValue() == "DEFAULT")
      mTextRenderStyle = TextRenderStyle::DEFAULT_RENDER_STYLE;
    else
    {
      DebugLogPrint("Incorrect render style %s used for text rendering.", aNode->Find("RenderStyle")->GetValue().ToCharArray());
      assert(!"Incorrect render style used for text rendering.");
    }
  }
  
  SetAnimated(animated);
  SetAnimation(startingAnimation);
  
  // Set bias after creating texture coordinates.
  mTexCoord->SetBias(0, xBias);
  mTexCoord->SetBias(1, yBias);
}

/**
 * @brief Copy surface
 * @param aNewOwner The new owner
 * @return Copied surface
 */
Component* Renderable::Clone(GameObject *aNewOwner) const
{
  return new Renderable(*this);
}

/**
 * @brief Make this object usable in LUA
 */
void Renderable::SerializeLUA()
{
  SLB::Class<Renderable>("Renderable")
    .inherits<Component>()
    .enumValue("Continuous", TextureCoordinateBehavior::CONTINUOUS)
    .set("SetAnimation", &Renderable::SetAnimation)
    .set("SetFrameByID", &Renderable::SetFrameByID)
    .set("SetAnimated", &Renderable::SetAnimated)
    .set("SetCurrentFrame", &Renderable::SetCurrentFrame)
    .set("SetAnimationSpeed", &Renderable::SetAnimationSpeed)
    .set("CurrentAnimatedCompleted", &Renderable::CurrentAnimationCompleted)
    .set("SetColor", &Renderable::SetColor)
    .set("AddOrEditProperty", &Renderable::AddOrEditProperty)
    .set("ClearProperties", &Renderable::ClearProperties);
}

/**
 * @brief Align objects
 * @param aTransform Transform of object
 * @param aSize Object size
 * @param aPosition Object position
 * @param aScale Object scale
 */
void Renderable::AlignmentHelper(Transform *aTransform, Vector3 const &aSize, Vector3 &aPosition, Vector3 &aScale)
{
  X_ALIGNMENT xAlign = aTransform->GetXAlignment();
  Y_ALIGNMENT yAlign = aTransform->GetYAlignment();
  Z_ALIGNMENT zAlign = aTransform->GetZAlignment();

  switch(xAlign)
  {
  case X_ALIGN_LEFT:
    aPosition.x += aSize.x * aScale.x;
    break;
  case X_ALIGN_RIGHT:
    aPosition.x -= aSize.x * aScale.x;
    break;
  default:
    break;
  }

  switch(yAlign)
  {
  case Y_ALIGN_TOP:
    aPosition.y += aSize.y * aScale.y;
    break;
  case Y_ALIGN_BOTTOM:
    aPosition.y -= aSize.y * aScale.y;
    break;
  default:
    break;
  }

  switch(zAlign)
  {
  case Z_ALIGN_FRONT:
    aPosition.z += aSize.z * aScale.z;
    break;
  case Z_ALIGN_BACK:
    aPosition.z -= aSize.z * aScale.z;
    break;
  default:
    break;
  }
}

