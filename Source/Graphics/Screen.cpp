#include "Screen.h"
#include "Transform.h"

bool ZSortPredicate(Surface *object1, Surface *object2)
{
  // Default values in case no owner found.
  float z1 = -0.9999f;
  float z2 = -0.9999f;
  if(object1->GetOwner())
  {
    z1 = object1->GetOwner()->GET<Transform>()->GetPosition().z;
  }
  if(object2->GetOwner())
  {
    z2 = object2->GetOwner()->GET<Transform>()->GetPosition().z;
  }

  // Right hand rule
  if(z1 < z2)
  {
    return true;
  }
  
  return false;
}

bool BatchSortPredicate(Surface *object1, Surface *object2)
{
  // Simple enough
  if(object1->GetTextureID() < object2->GetTextureID())
  {
    return true;
  }

  return false;
}

Screen::Screen() : mWidth(0), mHeight(0)
{
}

Screen::Screen(int aW, int aH) : mWidth(aW), mHeight(aH)
{
  mView.SetSize(Vector3(aW, aH, 0));
}

Screen::~Screen()
{

}

int Screen::GetWidth() const
{
  return mWidth;
}

int Screen::GetHeight() const
{
  return mHeight;
}

View &Screen::GetView()
{
	return mView;
}

// This assumes that objects of same texture ID are at the same z depth.
std::vector<Surface*> Screen::SortObjects(std::vector<Surface*> const &aObjects)
{
  std::vector<Surface*> ret = aObjects;
  std::sort(ret.begin(), ret.end(), BatchSortPredicate);
  std::sort(ret.begin(), ret.end(), ZSortPredicate);
  return ret;
}
