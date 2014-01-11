#include "Surface.h"
#include "GraphicsManager.h"
#include "ObjectManager.h"

Surface::Surface() : Component("Surface"), mTexCoord(NULL), mViewmode(VIEW_ABSOLUTE), mColor(1,1,1,1)
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component("Surface"), mTexCoord(NULL),
                                              mManager(aManager), mViewmode(VIEW_ABSOLUTE),
                                              mColor(1,1,1,1)
{
  
}

Surface::~Surface()
{
  GetManager()->RemoveSurface(this);
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
  
}

void Surface::Deserialize(Parser &aParser)
{
  bool animated = false;
  int numAnimations = 1;
  std::vector<int> numFrames;
  numFrames.push_back(1);
  
  if(aParser.Find("Surface", "AnimationCount") != "BadString")
  {
    numFrames.clear();
    numAnimations = Common::StringToFloat(aParser.Find("Surface", "AnimationCount"));
    numFrames = Common::StringToIntVector(aParser.Find("Surface", "FrameNumbers"));
    
    std::string isAnimated = aParser.Find("Surface", "Animated");
    if(isAnimated == "true")
      animated = true;
  }
  if(aParser.Find("Surface", "NoRender") != "BadString")
  {
    std::string norender = aParser.Find("Surface", "NoRender");
    if(norender == "true")
      mManager->RemoveSurface(this);
  }
  if(aParser.Find("Surface", "ColorR") != "BadString")
  {
    float red = Common::StringToFloat(aParser.Find("Surface", "ColorR"));
    float green = Common::StringToFloat(aParser.Find("Surface", "ColorG"));
    float blue = Common::StringToFloat(aParser.Find("Surface", "ColorB"));
    float alpha = Common::StringToFloat(aParser.Find("Surface", "ColorA"));

    mColor = Vector4(red, green, blue, alpha);
  }
  
  SetTextureCoordinateData(numAnimations, numFrames);
  SetAnimated(animated);
}
