#include "Surface.h"
#include "LUATypes.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "Transform.h"

#define DT (1.0f/60.0f)

int const Surface::sUID = Common::StringHashFunction("Surface");

Surface::Surface() : Component(Surface::sUID), mTexCoord(NULL), mViewmode(VIEW_ABSOLUTE),
                     mTextureSize(), mPrimaryColor(1,1,1,1), mNoRender(false), 
                     mMinFilter(SystemProperties::GetMinFilter()), mMagFilter(SystemProperties::GetMagFilter()), 
                     mLayer(0), mFileName(), mText(), mFontName(), mFontSize(0), mMaxTextWidth(0), mSecondaryColor(),
                     mOriginalSize(), mTextRenderStyle(DEFAULT_RENDER_STYLE), mScrollInfo(), mProperties()
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component(Surface::sUID), mTexCoord(NULL),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mTextureSize(), mPrimaryColor(1,1,1,1), mNoRender(false), 
                                              mMinFilter(SystemProperties::GetMinFilter()), mMagFilter(SystemProperties::GetMagFilter()),
                                              mLayer(0), mFileName(), mText(), mFontName(), mFontSize(0), 
                                              mMaxTextWidth(0), mSecondaryColor(), mOriginalSize(),
                                              mTextRenderStyle(DEFAULT_RENDER_STYLE), mScrollInfo(), mProperties()
{
}

Surface::~Surface()
{
  ClearProperties();
  mManager->RemoveSurface(this);
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
void Surface::LoadImage(HashString const &aName)
{
  assert(!"Not supported (Surface LoadImage)");
}

/**
 * @brief Asserts, please implement the platform specific implementation.
 * @param aText Nothing.
 * @return Nothing.
 */
void Surface::LoadText(HashString const &aText)
{
  assert(!"Not supported (Surface LoadText)");
}

/**
 * @brief Asserts, please implement the platform specific implementation.
 * @param aVertexShaderFilename Nothing.
 * @param aFragmentShaderFilename Nothing.
 */
void Surface::LoadShaders(HashString const &aVertexShaderFilename, HashString const &aFragmentShaderFilename)
{
  assert(!"Not supported (Surface LoadShaders)");
}

/**
 * @brief Get texture coordinate data (animation data)
 * @return Texture coordinate data.
 */
TextureCoordinates *Surface::GetTextureData() const
{
  return mTexCoord;
}

/**
 * @brief Set up animation data helper
 * @param aNumAnimations Total number of animations
 * @param aNumFrames Number of frames per animation
 * @param aAnimationSpeed Time between frames
 */
void Surface::SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const &aNumFrames, 
                                       std::vector<std::vector<float>> const &aAnimationSpeeds)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(mTextureSize.x, mTextureSize.y, aNumAnimations, aNumFrames, aAnimationSpeeds);
}

/**
 * @brief Set whether this surface is animated
 * @param aAnimated True is animated, false otherwise
 */
void Surface::SetAnimated(bool aAnimated)
{
  if(mTexCoord)
    mTexCoord->SetAnimated(aAnimated);
}

/**
 * @brief Get current animation
 * @param aAnimation Animation id to run
 * @param aBehavior Set behavior of animation (run once and stop or run and reset, etc.)
 */
void Surface::SetAnimation(int aAnimation, int aBehavior)
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
void Surface::SetCurrentFrame(int aFrame)
{
  if(mTexCoord)
    mTexCoord->SetCurrentFrame(aFrame);
}

/**
 * @brief Set frame by id in animation file. (Regardless of animation)
 * @param aFrameID The id to set to.
 */
void Surface::SetFrameByID(int aFrameID)
{
  if(mTexCoord)
    mTexCoord->SetFrameByID(aFrameID);
}

/**
 * @brief Set speed of current animation
 * @param aAnimationSpeed Time between frames (in seconds)
 */
void Surface::SetAnimationSpeed(float aAnimationSpeed)
{
  if(mTexCoord)
    mTexCoord->SetCurrentAnimationSpeed(aAnimationSpeed);
}

/**
 * @brief Force finish animation
 */
void Surface::FinishAnimation()
{
  if(mTexCoord)
    mTexCoord->Finish();
}

/**
 * @brief Check if current animation has iterated at least once
 * @return True if true.
 */
bool Surface::CurrentAnimationCompleted()
{
  if(mTexCoord)
    return mTexCoord->GetCompleted();
  return false;
}

/**
 * @brief Create scrolling reveal effect.
 * @param aScrollType HORIZONTAL or VERTICAL.
 * @param aGoalSize Goal size of the object (in world space).
 */
void Surface::CreateScrollEffect(ScrollType const& aScrollType, Vector3 const& aGoalSize)
{
  ScrollInfo scrollInfo;
  scrollInfo.mType = aScrollType;
  scrollInfo.mGoalSize = aGoalSize;
  mScrollInfo.push_back(scrollInfo);
}

/**
 * @brief Get properties container
 * @return Container of properties
 */
PropertyContainer const& Surface::GetProperties() const
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
void Surface::AddOrEditProperty(HashString const &aName, PropertyType const &aType, HashString const &aTargetValue, HashString const &aDefaultValue)
{
  PropertyContainerIt propertyEnd = mProperties.end();
  for(PropertyContainerIt it = mProperties.begin(); it != propertyEnd; ++it)
  {
    if((*it)->GetName() == aName)
    {
      (*it)->SetType(aType);
      (*it)->SetTargetValue(aTargetValue);
      (*it)->SetDefaultValue(aDefaultValue);
      return;
    }
  }
  mProperties.push_back(new SurfaceProperty(aName, aType, aTargetValue, aDefaultValue));
}

/**
 * @brief Clear properties for a surface.
 */
void Surface::ClearProperties()
{
  PropertyContainerIt propertyEnd = mProperties.end();
  for(PropertyContainerIt it = mProperties.begin(); it != propertyEnd; ++it)
  {
    delete *it;
  }
  mProperties.clear();
}

/**
 * @brief Simple update loop.
 */
void Surface::Update()
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
void Surface::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

/**
 * @brief Receive a message
 * @param aMessage Received message
 */
void Surface::ReceiveMessage(Message const &aMessage)
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
void Surface::Serialize(ParserNode *aNode)
{
  HashString const SURFACE = "Surface";
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
  surface->Place("AnimationCount", Common::IntToString(numanimations));
  for(int i = 0; i < numanimations; ++i)
  {
    animations.push_back(coords->GetAnimationFrameCounts(i));
    
    animationSpeeds.push_back(std::vector<float>());
    TextureCoordinates::SpeedContainer speedForAnimation = coords->GetAnimationHolds(i);
    for(TextureCoordinates::SpeedConstIT it = speedForAnimation.begin(); it != speedForAnimation.end(); ++it)
    {
      animationSpeeds[i].push_back(*it);
    }
  }

  if(animated)
  {
    surface->Place("FrameNumbers", Common::IntVectorToString(animations));
  }
  
  // Animation speeds
  surface->Place("AnimationSpeeds", "");
  HashString const ANIMATION = "Animation_";
  int curIndex = 0;
  ParserNode* animationSpeedsNode = surface->Find("AnimationSpeeds");
  for(std::vector<std::vector<float>>::iterator it = animationSpeeds.begin(); it != animationSpeeds.end(); ++it, ++curIndex)
  {
    HashString curNode = ANIMATION + Common::IntToString(curIndex);
    animationSpeedsNode->Place(curNode, Common::FloatVectorToString(*it));
  }
  
  // Everything else animation related
  surface->Place("Animated", Common::BoolToString(animated));
  surface->Place("NoRender", Common::BoolToString(mNoRender));
  surface->Place("StartingAnimation", Common::IntToString(currentAnimation));
  surface->Place("XBias", Common::FloatToString(mTexCoord->GetBias(0)));
  surface->Place("YBias", Common::FloatToString(mTexCoord->GetBias(1)));
  for(int i = 0; i < 4; ++i)
  {
    surface->Place(values[i], Common::IntToString(mPrimaryColor[i]));
  }
  
  surface->Place("MinFilter", mMinFilter);
  surface->Place("MagFilter", mMagFilter);
  surface->Place("Layer", Common::IntToString(mLayer));
  
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
      switch((*it)->GetType())
      {
      case PropertyType::INT1:
        type = "INT1";
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
      case PropertyType::FLOAT3:
        type = "FLOAT3";
        break;
      case PropertyType::FLOAT4:
        type = "FLOAT4";
        break;
      default:
        assert(!"Invalid PropertyType trying to be serialized.");
        break;
      }
      
      ParserNode* propertyNode = propertiesNode->Find(curNode);
      propertyNode->Place("Name", (*it)->GetName());
      propertyNode->Place("Type", type);
      propertyNode->Place("TargetValue", (*it)->GetTargetValue());
      propertyNode->Place("DefaultValue", (*it)->GetDefaultValue());
    }
  }
}

/**
 * @brief Read from file
 * @param aNode ParserNode to read from.
 */
void Surface::Deserialize(ParserNode *aNode)
{
  float const frameRate = GetManager()->GetOwningApp()->GetAppStep();
  bool animated = false;
  bool frameBased = false;
  int numAnimations = 1;
  int startingAnimation = 0;
  float xBias = 0;
  float yBias = 0;
  std::vector<TextureCoordinates::SpeedContainer> animationSpeed;
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
  }
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
      assert(!"Invalid value passed into ViewMode for Surface. (Surface.cpp)");  
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
      else if(type == "INT3")
        propertyType = PropertyType::INT3;
      else if(type == "INT4")
        propertyType = PropertyType::INT4;
      else if(type == "FLOAT1")
        propertyType = PropertyType::FLOAT1;
      else if(type == "FLOAT3")
        propertyType = PropertyType::FLOAT3;
      else if(type == "FLOAT4")
        propertyType = PropertyType::FLOAT4;
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
    xBias = aNode->Find("XBias")->GetValue().ToFloat();
  }
  if(aNode->Find("YBias"))
  {
    yBias = aNode->Find("YBias")->GetValue().ToFloat();
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
  
  SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
  SetAnimated(animated);
  SetAnimation(startingAnimation);
  
  // Set bias after creating texture coordinates.
  mTexCoord->SetBias(0, xBias);
  mTexCoord->SetBias(1, yBias);
}

/**
 * @brief Make this object usable in LUA
 */
void Surface::SerializeLUA()
{
  SLB::Class<Surface>("Surface")
    .inherits<Component>()
    .enumValue("Continuous", TextureCoordinateBehavior::CONTINUOUS)
    .set("SetAnimation", &Surface::SetAnimation)
    .set("SetFrameByID", &Surface::SetFrameByID)
    .set("SetAnimated", &Surface::SetAnimated)
    .set("SetCurrentFrame", &Surface::SetCurrentFrame)
    .set("SetAnimationSpeed", &Surface::SetAnimationSpeed)
    .set("CurrentAnimatedCompleted", &Surface::CurrentAnimationCompleted)
    .set("SetColor", &Surface::SetColor)
    .set("AddOrEditProperty", &Surface::AddOrEditProperty)
    .set("ClearProperties", &Surface::ClearProperties);
}
