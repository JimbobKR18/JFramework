#include "Surface.h"
#include "LUATypes.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "Transform.h"

#define DT (1.0f/60.0f)

int const Surface::sUID = Common::StringHashFunction("Surface");

Surface::Surface() : Component(Surface::sUID), mTexCoord(NULL), mViewmode(VIEW_ABSOLUTE),
                     mTextureSize(), mColor(1,1,1,1), mFileName(""), mNoRender(false), mScrollInfo()
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component(Surface::sUID), mTexCoord(NULL),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mTextureSize(), mColor(1,1,1,1), mFileName(""), mNoRender(false), mScrollInfo()
{
  
}

Surface::~Surface()
{
  GetManager()->RemoveSurface(this);
  if(mTexCoord)
    delete mTexCoord;
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
 * @param aFont Nothing.
 * @param aText Nothing.
 * @param aForegroundColor Nothing.
 * @param aBackgroundColor Nothing.
 * @param aSize Nothing.
 * @param aMaxWidth Nothing.
 * @return Nothing.
 */
Vector3 Surface::LoadText(HashString const &aFont, HashString const &aText, Vector4 const &aForegroundColor, Vector4 const &aBackgroundColor, int aSize, int aMaxWidth)
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
void Surface::SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames, float aAnimationSpeed)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(mTextureSize.x, mTextureSize.y, aNumAnimations, aNumFrames, aAnimationSpeed);
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
 * @param aRunOnce Iterates only once then stops if true.
 */
void Surface::SetAnimation(int aAnimation, bool aRunOnce)
{
  if(mTexCoord)
  {
    mTexCoord->SetCurrentAnimation(aAnimation);
    mTexCoord->SetRunOnce(aRunOnce);
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
    mTexCoord->SetAnimationSpeed(aAnimationSpeed);
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
}

/**
 * @brief Serialize to file
 * @param aParser File stream to write to.
 */
void Surface::Serialize(Parser &aParser)
{
  HashString const objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  HashString const SURFACE = "Surface";
  Root* object = aParser.Find(objectName);
  TextureCoordinates *coords = GetTextureData();
  std::vector<int> animations;
  bool animated = coords->GetAnimated();
  int numanimations = coords->GetNumberOfAnimations();
  float animationSpeed = coords->GetAnimationSpeed();
  int currentAnimation = coords->GetCurrentAnimation();
  char const* values[4] = { "ColorR",
                            "ColorG",
                            "ColorB",
                            "ColorA"};

  object->Place(objectName, SURFACE, "");
  Root* surface = object->Find(SURFACE);
  surface->Place(SURFACE, "AnimationCount", Common::IntToString(numanimations));
  for(int i = 0; i < numanimations; ++i)
  {
    animations.push_back(coords->GetAnimationFrameCounts(i));
  }

  if(animated)
  {
    surface->Place(SURFACE, "FrameNumbers", Common::IntVectorToString(animations));
  }
  surface->Place(SURFACE, "Animated", Common::BoolToString(animated));
  surface->Place(SURFACE, "AnimationSpeed", Common::FloatToString(animationSpeed));
  surface->Place(SURFACE, "NoRender", Common::BoolToString(mNoRender));
  surface->Place(SURFACE, "StartingAnimation", Common::IntToString(currentAnimation));
  for(int i = 0; i < 4; ++i)
  {
    surface->Place("Surface", values[i], Common::IntToString(mColor[i]));
  }
  
  HashString viewMode = (mViewmode == VIEW_ABSOLUTE) ? "Absolute" : "Relative";
  surface->Place(SURFACE, "ViewMode", viewMode.ToString());
}

/**
 * @brief Read from file
 * @param aParser File stream to read from
 */
void Surface::Deserialize(Parser &aParser)
{
  bool animated = false;
  int numAnimations = 1;
  int startingAnimation = 0;
  float animationSpeed = DT;
  std::vector<int> numFrames;
  numFrames.push_back(1);
  
  if(aParser.Find("Surface", "AnimationCount"))
  {
    Root* animationCount = aParser.Find("Surface", "AnimationCount");

    numFrames.clear();
    numAnimations = animationCount->GetValue().ToInt();
    numFrames = aParser.Find("Surface", "FrameNumbers")->GetValue().ToIntVector();
    
    bool isAnimated = aParser.Find("Surface", "Animated")->GetValue().ToBool();
    if(isAnimated)
      animated = true;
  }
  if(aParser.Find("Surface", "AnimationSpeed"))
  {
    // Optional parameter to change the animation speed.
    Root* animationSpeedNode = aParser.Find("Surface", "AnimationSpeed");
    animationSpeed = animationSpeedNode->GetValue().ToFloat();
  }
  if(aParser.Find("Surface", "NoRender"))
  {
    mNoRender = aParser.Find("Surface", "NoRender")->GetValue().ToBool();
    if(mNoRender)
      mManager->RemoveSurface(this);
  }
  if(aParser.Find("Surface", "ColorR"))
  {
    float red = aParser.Find("Surface", "ColorR")->GetValue().ToFloat();
    float green = aParser.Find("Surface", "ColorG")->GetValue().ToFloat();
    float blue = aParser.Find("Surface", "ColorB")->GetValue().ToFloat();
    float alpha = aParser.Find("Surface", "ColorA")->GetValue().ToFloat();

    mColor = Vector4(red, green, blue, alpha);
  }
  if(aParser.Find("Surface", "ViewMode"))
  {
    HashString viewMode = aParser.Find("Surface", "ViewMode")->GetValue();
    if(viewMode == "Absolute")
      mViewmode = VIEW_ABSOLUTE;
    else if(viewMode == "Relative")
      mViewmode = VIEW_RELATIVE_TO_CAMERA;
    else
      assert(!"Invalid value passed into ViewMode for Surface. (Surface.cpp)");  
  }
  if(aParser.Find("Surface", "StartingAnimation"))
  {
    startingAnimation = aParser.Find("Surface", "StartingAnimation")->GetValue().ToInt();
  }
  
  SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
  SetAnimated(animated);
  SetAnimation(startingAnimation);
}

/**
 * @brief Make this object usable in LUA
 */
void Surface::SerializeLUA()
{
  SLB::Class<Surface>("Surface")
    .set("SetAnimation", &Surface::SetAnimation)
    .set("SetFrameByID", &Surface::SetFrameByID)
    .set("SetAnimated", &Surface::SetAnimated)
    .set("SetCurrentFrame", &Surface::SetCurrentFrame)
    .set("SetAnimationSpeed", &Surface::SetAnimationSpeed)
    .set("CurrentAnimatedCompleted", &Surface::CurrentAnimationCompleted)
    .set("SetColor", &Surface::SetColor);
}
