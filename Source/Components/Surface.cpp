#include "Surface.h"
#include "GraphicsManager.h"

Surface::Surface() : Component("Surface"), mTexCoord(NULL)
{
  assert(!"Surface needs a graphicsmanager");
}

Surface::Surface(GraphicsManager *aManager) : Component("Surface"), mTexCoord(NULL),
                                              mManager(aManager)
{
  
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

void Surface::Update()
{
  if(mTexCoord)
    mTexCoord->Update(1.0f/60.0f);
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
  
  SetTextureCoordinateData(numAnimations, numFrames);
  SetAnimated(animated);
}
