#include "Surface.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"

#define DT (1.0f/60.0f)

Surface::Surface() : Component("Surface"), mTexCoord(NULL), mViewmode(VIEW_ABSOLUTE),
                     mTextureSize(), mColor(1,1,1,1), mFileName(""), mNoRender("false")
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component("Surface"), mTexCoord(NULL),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mTextureSize(), mColor(1,1,1,1), mFileName(""), mNoRender("false")
{
  
}

Surface::~Surface()
{
  GetManager()->RemoveSurface(this);
  if(mTexCoord)
    delete mTexCoord;
}

TextureCoordinates *Surface::GetTextureData() const
{
  return mTexCoord;
}

void Surface::SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames, float aAnimationSpeed)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(mTextureSize.x, mTextureSize.y, aNumAnimations, aNumFrames, aAnimationSpeed);
}

void Surface::SetAnimated(bool aAnimated)
{
  if(mTexCoord)
    mTexCoord->SetAnimated(aAnimated);
}

void Surface::SetAnimation(int aAnimation)
{
  if(mTexCoord)
    mTexCoord->SetCurrentAnimation(aAnimation);
}

void Surface::SetCurrentFrame(int aFrame)
{
  if(mTexCoord)
    mTexCoord->SetCurrentFrame(aFrame);
}

void Surface::SetFrameByID(int aFrameID)
{
  if(mTexCoord)
    mTexCoord->SetFrameByID(aFrameID);
}

bool Surface::CurrentAnimationCompleted()
{
  if(mTexCoord)
    return mTexCoord->GetCompleted();
  return false;
}

void Surface::Update()
{
  float dt = GetOwner()->GetManager()->GetOwningApp()->GetDT();

  if(mTexCoord)
    mTexCoord->Update(dt);
}

void Surface::SendMessage(Message const &aMessage)
{
  GetOwner()->ReceiveMessage(aMessage);
}

void Surface::ReceiveMessage(Message const &aMessage)
{
}

void Surface::Serialize(Parser &aParser)
{
  std::string objectName = std::string("Object_") + Common::IntToString(aParser.GetCurrentObjectIndex());
  Root* object = aParser.Find(objectName);
  TextureCoordinates *coords = GetTextureData();
  std::vector<int> animations;
  bool animated = coords->GetAnimated();
  int numanimations = coords->GetNumberofAnimations();
  float animationSpeed = coords->GetAnimationSpeed();
  char const* values[4] = { "ColorR",
                            "ColorG",
                            "ColorB",
                            "ColorA"};

  object->Place(objectName, "Surface", "");
  object->Place("Surface", "AnimationCount", Common::IntToString(numanimations));
  for(int i = 0; i < numanimations; ++i)
  {
    animations.push_back(coords->GetAnimationFrameCounts(i));
  }

  if(animated)
  {
    object->Place("Surface", "FrameNumbers", Common::IntVectorToString(animations));
  }
  object->Place("Surface", "Animated", Common::BoolToString(animated));
  object->Place("Surface", "AnimationSpeed", Common::FloatToString(animationSpeed));
  object->Place("Surface", "NoRender", Common::BoolToString(mNoRender));
  for(int i = 0; i < 4; ++i)
  {
    object->Place("Surface", values[i], Common::IntToString(mColor[i]));
  }
  
  HashString viewMode = (mViewmode == VIEW_ABSOLUTE) ? "Absolute" : "Relative";
  object->Place("Surface", "ViewMode", viewMode.ToString());
}

void Surface::Deserialize(Parser &aParser)
{
  bool animated = false;
  int numAnimations = 1;
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
  
  SetTextureCoordinateData(numAnimations, numFrames, animationSpeed);
  SetAnimated(animated);
}
