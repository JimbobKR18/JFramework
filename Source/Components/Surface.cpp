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
