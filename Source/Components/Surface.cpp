#include "Surface.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"

Surface::Surface() : Component("Surface"), mTexCoord(NULL), mViewmode(VIEW_ABSOLUTE),
                     mColor(1,1,1,1), mFileName(""), mNoRender("false")
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component("Surface"), mTexCoord(NULL),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mColor(1,1,1,1), mFileName(""), mNoRender("false")
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

void Surface::SetTextureCoordinateData(int const aNumAnimations, std::vector<int> const aNumFrames)
{
  if(mTexCoord)
    delete mTexCoord;
  
  mTexCoord = new TextureCoordinates(aNumAnimations, aNumFrames);
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

  object->Place("Surface", "NoRender", Common::BoolToString(mNoRender));
  for(int i = 0; i < 4; ++i)
  {
    object->Place("Surface", values[i], Common::IntToString(mColor[i]));
  }
}

void Surface::Deserialize(Parser &aParser)
{
  bool animated = false;
  int numAnimations = 1;
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
  
  SetTextureCoordinateData(numAnimations, numFrames);
  SetAnimated(animated);
}
